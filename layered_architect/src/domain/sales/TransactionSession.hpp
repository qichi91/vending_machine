#ifndef VENDING_MACHINE_DOMAIN_SALES_TRANSACTIONSESSION_HPP
#define VENDING_MACHINE_DOMAIN_SALES_TRANSACTIONSESSION_HPP

#include "SessionStatus.hpp"
#include "domain/inventory/SlotId.hpp"
#include "domain/sales/SessionId.hpp"
#include <optional>

namespace vending_machine {
namespace domain {

/**
 * @class TransactionSession
 * @brief 取引セッションを表すエンティティ
 *
 * 一連の取引において、選択された商品と現在の状態を保持します。
 * セッションIDによって識別されます。
 */
class TransactionSession {
public:
  /**
   * @brief コンストラクタ - 新しいセッションを開始
   * @param session_id セッションID
   */
  explicit TransactionSession(const SessionId &session_id);

  /**
   * @brief セッションIDを取得
   * @return セッションID
   */
  const SessionId &getSessionId() const;

  /**
   * @brief 現在のステータスを取得
   * @return セッションステータス
   */
  SessionStatus getStatus() const;

  /**
   * @brief 選択された商品のスロットIDを取得
   * @return スロットID（選択されていない場合はnullopt）
   */
  std::optional<SlotId> getSelectedSlotId() const;

  /**
   * @brief 商品を選択
   * @param slot_id 選択するスロットID
   * @throw std::domain_error 既に商品が選択されている場合
   */
  void selectProduct(const SlotId &slot_id);

  /**
   * @brief 決済待ち状態に遷移
   * @throw std::domain_error 商品が選択されていない場合
   */
  void markPaymentPending();

  /**
   * @brief 排出中状態に遷移
   * @throw std::domain_error 決済待ち状態でない場合
   */
  void markDispensing();

  /**
   * @brief 取引を完了
   * @throw std::domain_error 排出中状態でない場合
   */
  void complete();

  /**
   * @brief 取引をキャンセル
   */
  void cancel();

  /**
   * @brief セッションが終了しているか確認
   * @return 完了またはキャンセル済みの場合true
   */
  bool isFinished() const;

private:
  SessionId session_id_;                   ///< セッションID
  SessionStatus status_;                   ///< 現在の状態
  std::optional<SlotId> selected_slot_id_; ///< 選択された商品のスロットID
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_SALES_TRANSACTIONSESSION_HPP
