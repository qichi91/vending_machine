/**
 * @file SlotId.hpp
 * @brief SlotId Value Object - スロットの識別子
 *
 * @details
 * SlotIdは自動販売機のスロットを一意に識別する値オブジェクトです。
 * 1以上の正の整数値を持ちます。
 *
 * @author VendingMachine Team
 * @version 1.0.0
 */

#ifndef VENDING_MACHINE_DOMAIN_INVENTORY_SLOTID_HPP
#define VENDING_MACHINE_DOMAIN_INVENTORY_SLOTID_HPP

#include <stdexcept>

namespace vending_machine {
namespace domain {

/**
 * @class SlotId
 * @brief スロット識別子の値オブジェクト
 *
 * スロットを一意に識別する正の整数値。
 */
class SlotId {
public:
  /**
   * @brief コンストラクタ
   * @param value スロット番号（1以上）
   * @throw std::invalid_argument スロット番号が0以下の場合
   */
  explicit SlotId(int value);

  /**
   * @brief スロット番号を取得
   * @return スロット番号
   */
  int getValue() const;

  /**
   * @brief 等価演算子
   * @param other 比較対象
   * @return スロット番号が等しい場合true
   */
  bool operator==(const SlotId &other) const;

  /**
   * @brief 不等価演算子
   * @param other 比較対象
   * @return スロット番号が等しくない場合true
   */
  bool operator!=(const SlotId &other) const;

  /**
   * @brief 小なり演算子
   * @param other 比較対象
   * @return このスロット番号がotherより小さい場合true
   */
  bool operator<(const SlotId &other) const;

  /**
   * @brief 大なり演算子
   * @param other 比較対象
   * @return このスロット番号がotherより大きい場合true
   */
  bool operator>(const SlotId &other) const;

private:
  int value_; ///< スロット番号
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_INVENTORY_SLOTID_HPP
