#include "application/usecases/CashCollectionUseCase.hpp"
#include "src/domain/common/Money.hpp"
#include "src/domain/sales/TransactionRecord.hpp"
#include "src/infrastructure/repositories/InMemoryTransactionHistoryRepository.hpp"
#include <gtest/gtest.h>

namespace vending_machine {
namespace application {

class CashCollectionUseCaseTest : public ::testing::Test {
protected:
  void SetUp() override {
    use_case = std::make_unique<CashCollectionUseCase>(repository_);
  }

  infrastructure::InMemoryTransactionHistoryRepository repository_;
  std::unique_ptr<CashCollectionUseCase> use_case;
};

// テスト1: 取引なしの場合、売上は0円
TEST_F(CashCollectionUseCaseTest, EmptyRepositoryReturnsZeroRevenue) {
  EXPECT_EQ(0, use_case->getTotalRevenue().getRawValue());
}

// テスト2: 単一の取引記録から売上を集計
TEST_F(CashCollectionUseCaseTest, CanGetTotalRevenueFromSingleTransaction) {
  repository_.save(domain::TransactionRecord(
      domain::SalesId(1), domain::SlotId(1), domain::Price(120),
      domain::PaymentMethodType::CASH));

  EXPECT_EQ(120, use_case->getTotalRevenue().getRawValue());
}

// テスト3: 複数の取引記録から売上を集計
TEST_F(CashCollectionUseCaseTest, CanGetTotalRevenueFromMultipleTransactions) {
  repository_.save(domain::TransactionRecord(
      domain::SalesId(1), domain::SlotId(1), domain::Price(120),
      domain::PaymentMethodType::CASH));
  repository_.save(domain::TransactionRecord(
      domain::SalesId(2), domain::SlotId(2), domain::Price(150),
      domain::PaymentMethodType::CASH));
  repository_.save(domain::TransactionRecord(
      domain::SalesId(3), domain::SlotId(1), domain::Price(120),
      domain::PaymentMethodType::EMONEY));

  EXPECT_EQ(390, use_case->getTotalRevenue().getRawValue());
}

// テスト4: 売上を回収すると履歴がクリアされる
TEST_F(CashCollectionUseCaseTest, CollectCashClearsHistory) {
  repository_.save(domain::TransactionRecord(
      domain::SalesId(1), domain::SlotId(1), domain::Price(120),
      domain::PaymentMethodType::CASH));
  repository_.save(domain::TransactionRecord(
      domain::SalesId(2), domain::SlotId(2), domain::Price(150),
      domain::PaymentMethodType::CASH));

  domain::Money collected = use_case->collectCash();

  EXPECT_EQ(270, collected.getRawValue());
  EXPECT_EQ(0, use_case->getTotalRevenue().getRawValue());
}

// テスト5: 回収後の売上は0円
TEST_F(CashCollectionUseCaseTest, RevenueIsZeroAfterCollection) {
  repository_.save(domain::TransactionRecord(
      domain::SalesId(1), domain::SlotId(1), domain::Price(120),
      domain::PaymentMethodType::CASH));

  use_case->collectCash();

  EXPECT_EQ(0, use_case->getTotalRevenue().getRawValue());
}

// テスト6: 複数回回収できる
TEST_F(CashCollectionUseCaseTest, CanCollectCashMultipleTimes) {
  // 1回目の回収
  repository_.save(domain::TransactionRecord(
      domain::SalesId(1), domain::SlotId(1), domain::Price(100),
      domain::PaymentMethodType::CASH));

  domain::Money first_collection = use_case->collectCash();
  EXPECT_EQ(100, first_collection.getRawValue());

  // 2回目の回収前に新しい取引を追加
  repository_.save(domain::TransactionRecord(
      domain::SalesId(2), domain::SlotId(2), domain::Price(200),
      domain::PaymentMethodType::CASH));

  domain::Money second_collection = use_case->collectCash();
  EXPECT_EQ(200, second_collection.getRawValue());
}

} // namespace application
} // namespace vending_machine
