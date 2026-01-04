/**
 * @file Money.hpp
 * @brief Money Value Object - 金額を表す不変の値オブジェクト
 *
 * @details
 * Moneyは金額を表現する値オブジェクトです。
 * 以下の特性を持ちます:
 * - 不変性（Immutable）: 一度生成されたら値は変更されない
 * - 自己妥当性（Self-validation）: 負の値を持たない
 * - 演算の安全性: 加算・減算の結果も常に妥当な Money オブジェクト
 *
 * @author VendingMachine Team
 * @version 1.0.0
 */

#ifndef VENDING_MACHINE_DOMAIN_COMMON_MONEY_HPP
#define VENDING_MACHINE_DOMAIN_COMMON_MONEY_HPP

#include <stdexcept>

namespace vending_machine {
namespace domain {

/**
 * @class Money
 * @brief 金額を表す値オブジェクト
 *
 * 金額の計算（加算・減算）のロジックを内包し、不変性を保証します。
 * 計算結果は常に新しいインスタンスとして返されます。
 */
class Money {
public:
  /**
   * @brief コンストラクタ
   * @param amount 金額（円）
   * @throw std::invalid_argument 金額が負の場合
   */
  explicit Money(int amount);

  /**
   * @brief 金額の生の値を取得
   * @return 金額（円）
   */
  int getRawValue() const;

  /**
   * @brief 金額が0円かどうかを判定
   * @return 0円の場合true、それ以外false
   */
  bool isZero() const;

  /**
   * @brief 加算演算子
   * @param other 加算する金額
   * @return 加算結果の新しいMoneyオブジェクト
   */
  Money operator+(const Money &other) const;

  /**
   * @brief 減算演算子
   * @param other 減算する金額
   * @return 減算結果の新しいMoneyオブジェクト
   * @throw std::domain_error 減算結果が負になる場合
   */
  Money operator-(const Money &other) const;

  /**
   * @brief 等価演算子
   * @param other 比較対象
   * @return 金額が等しい場合true
   */
  bool operator==(const Money &other) const;

  /**
   * @brief 不等価演算子
   * @param other 比較対象
   * @return 金額が等しくない場合true
   */
  bool operator!=(const Money &other) const;

  /**
   * @brief 小なり演算子
   * @param other 比較対象
   * @return この金額がotherより小さい場合true
   */
  bool operator<(const Money &other) const;

  /**
   * @brief 小なりイコール演算子
   * @param other 比較対象
   * @return この金額がother以下の場合true
   */
  bool operator<=(const Money &other) const;

  /**
   * @brief 大なり演算子
   * @param other 比較対象
   * @return この金額がotherより大きい場合true
   */
  bool operator>(const Money &other) const;

  /**
   * @brief 大なりイコール演算子
   * @param other 比較対象
   * @return この金額がother以上の場合true
   */
  bool operator>=(const Money &other) const;

private:
  int amount_; ///< 金額（円）
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_COMMON_MONEY_HPP
