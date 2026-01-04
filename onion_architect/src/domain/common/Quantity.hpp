/**
 * @file Quantity.hpp
 * @brief Quantity Value Object - 在庫数を表す不変の値オブジェクト
 *
 * @details
 * Quantityは在庫数を表現する値オブジェクトです。
 * 以下の特性を持ちます:
 * - 不変性（Immutable）: 一度生成されたら値は変更されない
 * - 自己妥当性（Self-validation）: 0以上、最大収容数以下の値のみ許可
 * - 安全な操作: 増加・減少の結果も常に妥当な Quantity オブジェクト
 *
 * @author VendingMachine Team
 * @version 1.0.0
 */

#ifndef VENDING_MACHINE_DOMAIN_COMMON_QUANTITY_HPP
#define VENDING_MACHINE_DOMAIN_COMMON_QUANTITY_HPP

#include <stdexcept>

namespace vending_machine {
namespace domain {

/**
 * @class Quantity
 * @brief 在庫数を表す値オブジェクト
 *
 * 在庫の増減ロジックを内包し、不変性と境界値の妥当性を保証します。
 */
class Quantity {
public:
  /**
   * @brief 最大収容数（自販機の物理的な制約）
   */
  static constexpr int MAX_CAPACITY = 50;

  /**
   * @brief コンストラクタ
   * @param value 在庫数
   * @throw std::invalid_argument 在庫数が負、または最大収容数を超える場合
   */
  explicit Quantity(int value);

  /**
   * @brief 在庫数を取得
   * @return 在庫数
   */
  int getValue() const;

  /**
   * @brief 在庫が0かどうかを判定（売り切れ判定）
   * @return 0の場合true、それ以外false
   */
  bool isZero() const;

  /**
   * @brief 在庫を増加
   * @param amount 増加する数量
   * @return 増加後の新しいQuantityオブジェクト
   * @throw std::invalid_argument amountが負の場合
   * @throw std::domain_error 増加後の値が最大収容数を超える場合
   */
  Quantity increase(int amount) const;

  /**
   * @brief 在庫を減少
   * @param amount 減少する数量
   * @return 減少後の新しいQuantityオブジェクト
   * @throw std::invalid_argument amountが負の場合
   * @throw std::domain_error 減少後の値が負になる場合
   */
  Quantity decrease(int amount) const;

  /**
   * @brief 等価演算子
   * @param other 比較対象
   * @return 在庫数が等しい場合true
   */
  bool operator==(const Quantity &other) const;

  /**
   * @brief 不等価演算子
   * @param other 比較対象
   * @return 在庫数が等しくない場合true
   */
  bool operator!=(const Quantity &other) const;

  /**
   * @brief 小なり演算子
   * @param other 比較対象
   * @return この在庫数がotherより小さい場合true
   */
  bool operator<(const Quantity &other) const;

  /**
   * @brief 大なり演算子
   * @param other 比較対象
   * @return この在庫数がotherより大きい場合true
   */
  bool operator>(const Quantity &other) const;

private:
  int value_; ///< 在庫数
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_COMMON_QUANTITY_HPP
