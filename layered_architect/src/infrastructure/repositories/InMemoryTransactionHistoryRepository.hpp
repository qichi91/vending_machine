#ifndef VENDING_MACHINE_INFRASTRUCTURE_REPOSITORIES_INMEMORY_INMEMORY_TRANSACTION_HISTORY_HPP
#define VENDING_MACHINE_INFRASTRUCTURE_REPOSITORIES_INMEMORY_INMEMORY_TRANSACTION_HISTORY_HPP

#include "application/repositories/ITransactionHistoryRepository.hpp"
#include <algorithm>
#include <vector>

namespace vending_machine {
namespace infrastructure {

/**
 * @class InMemoryTransactionHistoryRepository
 * @brief トランザクション履歴のメモリ内実装
 *
 * すべてのトランザクションをメモリ（std::vector）に保持します。
 * アプリケーション実行中のみ有効です。
 */
class InMemoryTransactionHistoryRepository
    : public application::ITransactionHistoryRepository {
public:
  /**
   * @brief コンストラクタ
   */
  InMemoryTransactionHistoryRepository() = default;

  /**
   * @brief トランザクションを保存
   */
  void save(const domain::TransactionRecord &record) override;

  /**
   * @brief すべてのトランザクション履歴を取得
   */
  std::vector<domain::TransactionRecord> getAll() const override;

  /**
   * @brief 指定スロットのトランザクション履歴を取得
   */
  std::vector<domain::TransactionRecord>
  getBySlotId(const domain::SlotId &slot_id) const override;

  /**
   * @brief 売上集計
   */
  domain::Money getTotalRevenue() const override;

  /**
   * @brief 履歴をクリア
   */
  void clear() override;

private:
  std::vector<domain::TransactionRecord> records_;
};

} // namespace infrastructure
} // namespace vending_machine

#endif // VENDING_MACHINE_INFRASTRUCTURE_REPOSITORIES_INMEMORY_INMEMORY_TRANSACTION_HISTORY_HPP
