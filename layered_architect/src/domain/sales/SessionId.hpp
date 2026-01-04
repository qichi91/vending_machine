#ifndef VENDING_MACHINE_DOMAIN_SALES_SESSIONID_HPP
#define VENDING_MACHINE_DOMAIN_SALES_SESSIONID_HPP

#include <stdexcept>

namespace vending_machine {
namespace domain {

/**
 * @class SessionId
 * @brief 取引セッションを識別する値オブジェクト
 *
 * TransactionSessionを一意に識別します。
 * 正の整数値を保持します。
 */
class SessionId {
public:
  /**
   * @brief コンストラクタ
   * @param value セッションID（1以上）
   * @throw std::invalid_argument 0以下の値が指定された場合
   */
  explicit SessionId(int value);

  /**
   * @brief セッションIDの値を取得
   * @return セッションID
   */
  int getValue() const;

  /**
   * @brief 等価性を比較
   * @param other 比較対象
   * @return 等しい場合true
   */
  bool operator==(const SessionId &other) const;

  /**
   * @brief 不等価性を比較
   * @param other 比較対象
   * @return 等しくない場合true
   */
  bool operator!=(const SessionId &other) const;

private:
  int value_; ///< セッションID
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_SALES_SESSIONID_HPP
