/**
 * @file Price.hpp
 * @brief Price Value Object - 商品価格を表す不変の値オブジェクト
 *
 * @details
 * Priceは商品の価格を表現する値オブジェクトです。
 * Moneyと似ていますが、「商品の価格」という文脈を持ちます。
 * 以下の特性を持ちます:
 * - 不変性（Immutable）: 一度生成されたら値は変更されない
 * - 自己妥当性（Self-validation）: 負の値を持たない
 * - Money型との相互運用性: 支払い判定のためにMoneyと比較可能
 *
 * @author VendingMachine Team
 * @version 1.0.0
 */

#ifndef VENDING_MACHINE_DOMAIN_COMMON_PRICE_HPP
#define VENDING_MACHINE_DOMAIN_COMMON_PRICE_HPP

#include <stdexcept>

namespace vending_machine {
namespace domain {

/**
 * @class Price
 * @brief 商品価格を表す値オブジェクト
 *
 * 商品の販売価格を表現し、Money型と比較可能です。
 */
class Price {
public:
  /**
   * @brief コンストラクタ
   * @param amount 価格（円）
   * @throw std::invalid_argument 価格が負の場合
   */
  explicit Price(int amount);

  /**
   * @brief 価格の生の値を取得
   * @return 価格（円）
   */
  int getRawValue() const;

  /**
   * @brief 等価演算子
   * @param other 比較対象
   * @return 価格が等しい場合true
   */
  bool operator==(const Price &other) const;

  /**
   * @brief 不等価演算子
   * @param other 比較対象
   * @return 価格が等しくない場合true
   */
  bool operator!=(const Price &other) const;

  /**
   * @brief 小なり演算子
   * @param other 比較対象
   * @return この価格がotherより小さい場合true
   */
  bool operator<(const Price &other) const;

  /**
   * @brief 小なりイコール演算子
   * @param other 比較対象
   * @return この価格がother以下の場合true
   */
  bool operator<=(const Price &other) const;

  /**
   * @brief 大なり演算子
   * @param other 比較対象
   * @return この価格がotherより大きい場合true
   */
  bool operator>(const Price &other) const;

  /**
   * @brief 大なりイコール演算子
   * @param other 比較対象
   * @return この価格がother以上の場合true
   */
  bool operator>=(const Price &other) const;

private:
  int amount_; ///< 価格（円）
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_COMMON_PRICE_HPP
