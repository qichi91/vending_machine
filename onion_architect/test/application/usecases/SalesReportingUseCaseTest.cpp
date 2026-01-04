#include "src/application/usecases/SalesReportingUseCase.hpp"
#include "src/domain/common/Money.hpp"
#include "src/domain/sales/TransactionRecord.hpp"
#include "src/infrastructure/repositories/InMemoryTransactionHistoryRepository.hpp"
#include <gtest/gtest.h>

namespace vending_machine {
namespace application {

class SalesReportingUseCaseTest : public ::testing::Test {
protected:
  void SetUp() override {
    use_case = std::make_unique<SalesReportingUseCase>(repository_);
  }

  infrastructure::InMemoryTransactionHistoryRepository repository_;
  std::unique_ptr<SalesReportingUseCase> use_case;
};

// テスト1: 空のリポジトリではレポートが空
TEST_F(SalesReportingUseCaseTest, EmptyRepositoryReturnsEmptyReports) {
  auto slot_report = use_case->generateSlotSalesReport();
  EXPECT_EQ(0, slot_report.size());

  auto payment_report = use_case->generatePaymentMethodReport();
  EXPECT_EQ(0, payment_report.size());

  EXPECT_EQ(0, use_case->getTotalTransactionCount());
}

// テスト2: スロット別売上レポートの生成
TEST_F(SalesReportingUseCaseTest, GenerateSlotSalesReport) {
  repository_.save(domain::TransactionRecord(
      domain::SalesId(1), domain::SlotId(1), domain::Price(120),
      domain::PaymentMethodType::CASH));
  repository_.save(domain::TransactionRecord(
      domain::SalesId(2), domain::SlotId(1), domain::Price(120),
      domain::PaymentMethodType::CASH));
  repository_.save(domain::TransactionRecord(
      domain::SalesId(3), domain::SlotId(2), domain::Price(150),
      domain::PaymentMethodType::EMONEY));

  auto reports = use_case->generateSlotSalesReport();

  EXPECT_EQ(2, reports.size());

  // スロット1: 2回、240円
  auto slot1_report = std::find_if(
      reports.begin(), reports.end(),
      [](const SlotSalesReport &r) { return r.slot_id.getValue() == 1; });
  ASSERT_NE(slot1_report, reports.end());
  EXPECT_EQ(2, slot1_report->transaction_count);
  EXPECT_EQ(240, slot1_report->total_revenue.getRawValue());

  // スロット2: 1回、150円
  auto slot2_report = std::find_if(
      reports.begin(), reports.end(),
      [](const SlotSalesReport &r) { return r.slot_id.getValue() == 2; });
  ASSERT_NE(slot2_report, reports.end());
  EXPECT_EQ(1, slot2_report->transaction_count);
  EXPECT_EQ(150, slot2_report->total_revenue.getRawValue());
}

// テスト3: 決済方法別売上レポートの生成
TEST_F(SalesReportingUseCaseTest, GeneratePaymentMethodReport) {
  repository_.save(domain::TransactionRecord(
      domain::SalesId(1), domain::SlotId(1), domain::Price(120),
      domain::PaymentMethodType::CASH));
  repository_.save(domain::TransactionRecord(
      domain::SalesId(2), domain::SlotId(2), domain::Price(150),
      domain::PaymentMethodType::CASH));
  repository_.save(domain::TransactionRecord(
      domain::SalesId(3), domain::SlotId(1), domain::Price(120),
      domain::PaymentMethodType::EMONEY));

  auto reports = use_case->generatePaymentMethodReport();

  EXPECT_EQ(2, reports.size());

  // CASH: 2回、270円
  auto cash_report = std::find_if(
      reports.begin(), reports.end(), [](const PaymentMethodReport &r) {
        return r.payment_method == domain::PaymentMethodType::CASH;
      });
  ASSERT_NE(cash_report, reports.end());
  EXPECT_EQ(2, cash_report->transaction_count);
  EXPECT_EQ(270, cash_report->total_revenue.getRawValue());

  // EMONEY: 1回、120円
  auto emoney_report = std::find_if(
      reports.begin(), reports.end(), [](const PaymentMethodReport &r) {
        return r.payment_method == domain::PaymentMethodType::EMONEY;
      });
  ASSERT_NE(emoney_report, reports.end());
  EXPECT_EQ(1, emoney_report->transaction_count);
  EXPECT_EQ(120, emoney_report->total_revenue.getRawValue());
}

// テスト4: 特定スロットの売上取得
TEST_F(SalesReportingUseCaseTest, GetRevenueBySlot) {
  repository_.save(domain::TransactionRecord(
      domain::SalesId(1), domain::SlotId(1), domain::Price(120),
      domain::PaymentMethodType::CASH));
  repository_.save(domain::TransactionRecord(
      domain::SalesId(2), domain::SlotId(1), domain::Price(150),
      domain::PaymentMethodType::CASH));
  repository_.save(domain::TransactionRecord(
      domain::SalesId(3), domain::SlotId(2), domain::Price(100),
      domain::PaymentMethodType::EMONEY));

  domain::Money slot1_revenue = use_case->getRevenueBySlot(domain::SlotId(1));
  EXPECT_EQ(270, slot1_revenue.getRawValue());

  domain::Money slot2_revenue = use_case->getRevenueBySlot(domain::SlotId(2));
  EXPECT_EQ(100, slot2_revenue.getRawValue());
}

// テスト5: 存在しないスロットの売上は0円
TEST_F(SalesReportingUseCaseTest, NonExistentSlotReturnsZero) {
  repository_.save(domain::TransactionRecord(
      domain::SalesId(1), domain::SlotId(1), domain::Price(120),
      domain::PaymentMethodType::CASH));

  domain::Money slot99_revenue = use_case->getRevenueBySlot(domain::SlotId(99));
  EXPECT_EQ(0, slot99_revenue.getRawValue());
}

// テスト6: 取引総数の取得
TEST_F(SalesReportingUseCaseTest, GetTotalTransactionCount) {
  EXPECT_EQ(0, use_case->getTotalTransactionCount());

  repository_.save(domain::TransactionRecord(
      domain::SalesId(1), domain::SlotId(1), domain::Price(120),
      domain::PaymentMethodType::CASH));
  EXPECT_EQ(1, use_case->getTotalTransactionCount());

  repository_.save(domain::TransactionRecord(
      domain::SalesId(2), domain::SlotId(2), domain::Price(150),
      domain::PaymentMethodType::EMONEY));
  EXPECT_EQ(2, use_case->getTotalTransactionCount());
}

// テスト7: 複数スロット・複数決済方法の複合レポート
TEST_F(SalesReportingUseCaseTest, ComplexScenarioReport) {
  // スロット1: CASH 2回、EMONEY 1回
  repository_.save(domain::TransactionRecord(
      domain::SalesId(1), domain::SlotId(1), domain::Price(100),
      domain::PaymentMethodType::CASH));
  repository_.save(domain::TransactionRecord(
      domain::SalesId(2), domain::SlotId(1), domain::Price(100),
      domain::PaymentMethodType::CASH));
  repository_.save(domain::TransactionRecord(
      domain::SalesId(3), domain::SlotId(1), domain::Price(100),
      domain::PaymentMethodType::EMONEY));

  // スロット2: EMONEY 2回
  repository_.save(domain::TransactionRecord(
      domain::SalesId(4), domain::SlotId(2), domain::Price(150),
      domain::PaymentMethodType::EMONEY));
  repository_.save(domain::TransactionRecord(
      domain::SalesId(5), domain::SlotId(2), domain::Price(150),
      domain::PaymentMethodType::EMONEY));

  // スロット別レポート検証
  auto slot_reports = use_case->generateSlotSalesReport();
  EXPECT_EQ(2, slot_reports.size());

  // 決済方法別レポート検証
  auto payment_reports = use_case->generatePaymentMethodReport();
  EXPECT_EQ(2, payment_reports.size());

  auto cash_report =
      std::find_if(payment_reports.begin(), payment_reports.end(),
                   [](const PaymentMethodReport &r) {
                     return r.payment_method == domain::PaymentMethodType::CASH;
                   });
  EXPECT_EQ(2, cash_report->transaction_count);
  EXPECT_EQ(200, cash_report->total_revenue.getRawValue());

  auto emoney_report = std::find_if(
      payment_reports.begin(), payment_reports.end(),
      [](const PaymentMethodReport &r) {
        return r.payment_method == domain::PaymentMethodType::EMONEY;
      });
  EXPECT_EQ(3, emoney_report->transaction_count);
  EXPECT_EQ(400, emoney_report->total_revenue.getRawValue());

  // 取引総数
  EXPECT_EQ(5, use_case->getTotalTransactionCount());
}

} // namespace application
} // namespace vending_machine
