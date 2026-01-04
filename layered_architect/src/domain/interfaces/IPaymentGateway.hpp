#ifndef VENDING_MACHINE_DOMAIN_INTERFACES_IPAYMENTGATEWAY_HPP
#define VENDING_MACHINE_DOMAIN_INTERFACES_IPAYMENTGATEWAY_HPP

namespace vending_machine {
namespace domain {

class Price;

/**
 * @enum PaymentStatus
 * @brief 電子決済の状態
 */
enum class PaymentStatus {
  PENDING,    ///< 決済待ち
  AUTHORIZED, ///< 承認済み
  FAILED,     ///< 失敗
  CANCELLED   ///< キャンセル
};

/**
 * @class IPaymentGateway
 * @brief 電子決済ゲートウェイのインターフェース
 *
 * 外部決済システムとの通信を抽象化します。
 */
class IPaymentGateway {
public:
  virtual ~IPaymentGateway() = default;

  /**
   * @brief 決済を要求
   * @param price 決済金額
   */
  virtual void requestPayment(const Price &price) = 0;

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

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_INTERFACES_IPAYMENTGATEWAY_HPP
