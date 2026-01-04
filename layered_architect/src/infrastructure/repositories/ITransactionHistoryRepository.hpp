#ifndef VENDING_MACHINE_INFRASTRUCTURE_REPOSITORIES_ITRANSACTIONHISTORY_HPP
#define VENDING_MACHINE_INFRASTRUCTURE_REPOSITORIES_ITRANSACTIONHISTORY_HPP

#include "domain/sales/TransactionRecord.hpp"
#include <vector>

namespace vending_machine {
namespace infrastructure {

/**
 * @interface ITransactionHistoryRepository
 * @brief トランザクション履歴の永続化インターフェース
 *
 * 実行されたトランザクションをリポジトリに保存・取得します。
 */
class ITransactionHistoryRepository {
public:
  virtual ~ITransactionHistoryRepository() = default;

  /**
   * @brief トランザクションを保存
   * @param record 保存するトランザクションレコード
   */
  virtual void save(const domain::TransactionRecord &record) = 0;

  /**
   * @brief すべてのトランザクション履歴を取得
   * @return トランザクションレコードのリスト（タイムスタンプ昇順）
   */
  virtual std::vector<domain::TransactionRecord> getAll() const = 0;

  /**
   * @brief 指定スロットのトランザクション履歴を取得
   * @param slot_id スロットID
   * @return 該当するトランザクションレコードのリスト
   */
  virtual std::vector<domain::TransactionRecord>
  getBySlotId(const domain::SlotId &slot_id) const = 0;

  /**
   * @brief 売上集計（すべてのトランザクションの合計）
   * @return 売上合計
   */
  virtual domain::Money getTotalRevenue() const = 0;

  /**
   * @brief 履歴をクリア（現金回収時など）
   */
  virtual void clear() = 0;
};

} // namespace infrastructure
} // namespace vending_machine

#endif // VENDING_MACHINE_INFRASTRUCTURE_REPOSITORIES_ITRANSACTIONHISTORY_HPP
