#ifndef VENDING_MACHINE_INFRASTRUCTURE_INTERFACES_IPAYMENT_GATEWAY_HPP
#define VENDING_MACHINE_INFRASTRUCTURE_INTERFACES_IPAYMENT_GATEWAY_HPP

namespace vending_machine {

namespace domain {
class Price;
}

namespace infrastructure {

/**
 * @enum PaymentStatus
 * @brief 決済状態
 */
enum class PaymentStatus {
  Pending,    ///< 決済待ち
  Authorized, ///< 承認済み
  Failed,     ///< 失敗
  Cancelled   ///< キャンセル
};

/**
 * @class IPaymentGateway
 * @brief 決済ゲートウェイのインターフェース
 *
 * 外部決済システムとの連携を抽象化します。
 * Infrastructure層で定義されます（標準的なレイヤードアーキテクチャ）。
 */
class IPaymentGateway {
public:
  virtual ~IPaymentGateway() = default;

  /**
   * @brief 決済を要求
   * @param price 決済金額
   */
  virtual void requestPayment(const domain::Price &price) = 0;

  /**
   * @brief 決済をキャンセル
   */
  virtual void cancelPayment() = 0;

  /**
   * @brief 決済ステータスを取得
   * @return 決済状態
   */
  virtual PaymentStatus getPaymentStatus() const = 0;
};

} // namespace infrastructure
} // namespace vending_machine

#endif // VENDING_MACHINE_INFRASTRUCTURE_INTERFACES_IPAYMENT_GATEWAY_HPP
