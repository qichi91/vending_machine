#ifndef VENDING_MACHINE_DOMAIN_SALES_SALESID_HPP
#define VENDING_MACHINE_DOMAIN_SALES_SALESID_HPP

#include <stdexcept>

namespace vending_machine {
namespace domain {

/**
 * @class SalesId
 * @brief 販売管理集約を識別する値オブジェクト
 *
 * Sales集約ルートを一意に識別します。
 */
class SalesId {
public:
  /**
   * @brief コンストラクタ
   * @param value 販売管理ID（1以上）
   * @throw std::invalid_argument 0以下の値が指定された場合
   */
  explicit SalesId(int value);

  /**
   * @brief 販売管理IDの値を取得
   * @return 販売管理ID
   */
  int getValue() const;

  /**
   * @brief 等価性を比較
   * @param other 比較対象
   * @return 等しい場合true
   */
  bool operator==(const SalesId &other) const;

  /**
   * @brief 不等価性を比較
   * @param other 比較対象
   * @return 等しくない場合true
   */
  bool operator!=(const SalesId &other) const;

private:
  int value_; ///< 販売管理ID
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_SALES_SALESID_HPP
