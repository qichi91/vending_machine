#ifndef VENDING_MACHINE_DOMAIN_SALES_SESSIONSTATUS_HPP
#define VENDING_MACHINE_DOMAIN_SALES_SESSIONSTATUS_HPP

namespace vending_machine {
namespace domain {

/**
 * @enum SessionStatus
 * @brief 取引セッションの状態
 */
enum class SessionStatus {
  PRODUCT_SELECTING, ///< 商品選択中
  PAYMENT_PENDING,   ///< 決済待ち
  DISPENSING,        ///< 商品排出中
  COMPLETED,         ///< 取引完了
  CANCELLED          ///< キャンセル済み
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_SALES_SESSIONSTATUS_HPP
