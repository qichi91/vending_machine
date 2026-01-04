/**
 * @file CashCollectionUseCase.hpp
 * @brief CashCollectionUseCase - 売上金回収のユースケース
 *
 * @details
 * CashCollectionUseCaseは、自動販売機の売上金を回収し、取引履歴をクリアする
 * オペレーションを管理するユースケースです。
 *
 * @author VendingMachine Team
 * @version 1.0.0
 */

#ifndef VENDING_MACHINE_APPLICATION_USECASES_CASH_COLLECTION_USECASE_HPP
#define VENDING_MACHINE_APPLICATION_USECASES_CASH_COLLECTION_USECASE_HPP

#include "domain/common/Money.hpp"
#include "infrastructure/interfaces/ITransactionHistoryRepository.hpp"

namespace vending_machine {
namespace application {

/**
 * @class CashCollectionUseCase
 * @brief 売上金回収を管理するユースケース
 *
 * 取引履歴から売上を集計し、回収処理を実施します。
 */
class CashCollectionUseCase {
public:
  /**
   * @brief コンストラクタ
   * @param transaction_history 取引履歴リポジトリ
   */
  explicit CashCollectionUseCase(
      infrastructure::ITransactionHistoryRepository &transaction_history);

  /**
   * @brief 売上金の合計を取得
   * @return 売上金の合計
   */
  domain::Money getTotalRevenue() const;

  /**
   * @brief 売上金を回収して履歴をクリア
   * @return 回収した売上金の合計
   */
  domain::Money collectCash();

private:
  infrastructure::ITransactionHistoryRepository &transaction_history_;
};

} // namespace application
} // namespace vending_machine

#endif // VENDING_MACHINE_APPLICATION_USECASES_CASH_COLLECTION_USECASE_HPP
