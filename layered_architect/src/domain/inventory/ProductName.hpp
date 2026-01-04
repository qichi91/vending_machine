/**
 * @file ProductName.hpp
 * @brief ProductName Value Object - 商品名
 *
 * @details
 * ProductNameは商品の名前を表現する値オブジェクトです。
 * 空文字列を許可するかどうかはビジネス要件に依存しますが、
 * ここでは空でない文字列のみを許可します。
 *
 * @author VendingMachine Team
 * @version 1.0.0
 */

#ifndef VENDING_MACHINE_DOMAIN_INVENTORY_PRODUCTNAME_HPP
#define VENDING_MACHINE_DOMAIN_INVENTORY_PRODUCTNAME_HPP

#include <stdexcept>
#include <string>

namespace vending_machine {
namespace domain {

/**
 * @class ProductName
 * @brief 商品名の値オブジェクト
 *
 * 商品名の妥当性を保証します。
 */
class ProductName {
public:
  /**
   * @brief コンストラクタ
   * @param name 商品名
   * @throw std::invalid_argument 商品名が空の場合
   */
  explicit ProductName(const std::string &name);

  /**
   * @brief 商品名を取得
   * @return 商品名
   */
  const std::string &getValue() const;

  /**
   * @brief 等価演算子
   * @param other 比較対象
   * @return 商品名が等しい場合true
   */
  bool operator==(const ProductName &other) const;

  /**
   * @brief 不等価演算子
   * @param other 比較対象
   * @return 商品名が等しくない場合true
   */
  bool operator!=(const ProductName &other) const;

private:
  std::string name_; ///< 商品名
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_INVENTORY_PRODUCTNAME_HPP
