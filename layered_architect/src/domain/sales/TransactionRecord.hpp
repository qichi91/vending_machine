#ifndef VENDING_MACHINE_DOMAIN_TRANSACTION_RECORD_HPP
#define VENDING_MACHINE_DOMAIN_TRANSACTION_RECORD_HPP

#include "domain/common/Money.hpp"
#include "domain/common/Price.hpp"
#include "domain/inventory/SlotId.hpp"
#include "domain/sales/SalesId.hpp"
#include <chrono>
#include <string>

namespace vending_machine {
namespace domain {

/**
 * @enum PaymentMethodType
 * @brief 決済方法
 */
enum class PaymentMethodType {
  CASH,  ///< 現金
  EMONEY ///< 電子マネー
};

/**
 * @class TransactionRecord
 * @brief トランザクション履歴レコード
 *
 * 実行された各取引の情報を不変レコードとして保持します。
 */
class TransactionRecord {
public:
  /**
   * @brief コンストラクタ（タイムスタンプ自動）
   * @param sales_id 販売ID
   * @param slot_id スロットID
   * @param price 商品価格
   * @param payment_method 決済方法
   */
  TransactionRecord(const SalesId &sales_id, const SlotId &slot_id,
                    const Price &price, PaymentMethodType payment_method);

  /**
   * @brief コンストラクタ（タイムスタンプ指定）
   * @param sales_id 販売ID
   * @param slot_id スロットID
   * @param price 商品価格
   * @param payment_method 決済方法
   * @param timestamp タイムスタンプ
   */
  TransactionRecord(const SalesId &sales_id, const SlotId &slot_id,
                    const Price &price, PaymentMethodType payment_method,
                    std::chrono::system_clock::time_point timestamp);

  /**
   * @brief 販売IDを取得
   */
  const SalesId &getSalesId() const;

  /**
   * @brief スロットIDを取得
   */
  const SlotId &getSlotId() const;

  /**
   * @brief 商品価格を取得
   */
  const Price &getPrice() const;

  /**
   * @brief 決済方法を取得
   */
  PaymentMethodType getPaymentMethod() const;

  /**
   * @brief タイムスタンプを取得
   */
  std::chrono::system_clock::time_point getTimestamp() const;

private:
  SalesId sales_id_;
  SlotId slot_id_;
  Price price_;
  PaymentMethodType payment_method_;
  std::chrono::system_clock::time_point timestamp_;
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_TRANSACTION_RECORD_HPP
