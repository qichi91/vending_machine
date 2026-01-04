/**
 * @file SalesReportingUseCase.hpp
 * @brief SalesReportingUseCase - 売上レポート生成のユースケース
 *
 * @details
 * SalesReportingUseCaseは、取引履歴から各種売上レポートを生成する
 * ユースケースです。スロット別、決済方法別などの集計が可能です。
 *
 * @author VendingMachine Team
 * @version 1.0.0
 */

#ifndef VENDING_MACHINE_APPLICATION_USECASES_SALES_REPORTING_USECASE_HPP
#define VENDING_MACHINE_APPLICATION_USECASES_SALES_REPORTING_USECASE_HPP

#include "application/repositories/ITransactionHistoryRepository.hpp"
#include "domain/common/Money.hpp"
#include "domain/inventory/SlotId.hpp"
#include "domain/sales/TransactionRecord.hpp"
#include <map>
#include <vector>

namespace vending_machine {
namespace application {

/**
 * @struct SlotSalesReport
 * @brief スロット別売上レポート
 */
struct SlotSalesReport {
  domain::SlotId slot_id;      ///< スロットID
  int transaction_count;       ///< 取引回数
  domain::Money total_revenue; ///< 売上合計

  SlotSalesReport(const domain::SlotId &id, int count,
                  const domain::Money &revenue)
      : slot_id(id), transaction_count(count), total_revenue(revenue) {}
};

/**
 * @struct PaymentMethodReport
 * @brief 決済方法別売上レポート
 */
struct PaymentMethodReport {
  domain::PaymentMethodType payment_method; ///< 決済方法
  int transaction_count;                    ///< 取引回数
  domain::Money total_revenue;              ///< 売上合計

  PaymentMethodReport(domain::PaymentMethodType method, int count,
                      const domain::Money &revenue)
      : payment_method(method), transaction_count(count),
        total_revenue(revenue) {}
};

/**
 * @class SalesReportingUseCase
 * @brief 売上レポート生成を管理するユースケース
 *
 * 取引履歴から各種売上レポートを生成します。
 */
class SalesReportingUseCase {
public:
  /**
   * @brief コンストラクタ
   * @param transaction_history 取引履歴リポジトリ
   */
  explicit SalesReportingUseCase(
      ITransactionHistoryRepository &transaction_history);

  /**
   * @brief スロット別売上レポートを生成
   * @return スロット別売上レポートのリスト
   */
  std::vector<SlotSalesReport> generateSlotSalesReport() const;

  /**
   * @brief 決済方法別売上レポートを生成
   * @return 決済方法別売上レポートのリスト
   */
  std::vector<PaymentMethodReport> generatePaymentMethodReport() const;

  /**
   * @brief 特定スロットの売上を取得
   * @param slot_id スロットID
   * @return スロットの売上合計
   */
  domain::Money getRevenueBySlot(const domain::SlotId &slot_id) const;

  /**
   * @brief 取引総数を取得
   * @return 取引総数
   */
  int getTotalTransactionCount() const;

private:
  ITransactionHistoryRepository &transaction_history_;
};

} // namespace application
} // namespace vending_machine

#endif // VENDING_MACHINE_APPLICATION_USECASES_SALES_REPORTING_USECASE_HPP
