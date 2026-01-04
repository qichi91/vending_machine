/**
 * @file ProductInfo.hpp
 * @brief ProductInfo Value Object - 商品情報
 *
 * @details
 * ProductInfoは商品の基本情報（名前と価格）を保持する値オブジェクトです。
 * 複数のスロットが同じ商品を扱う場合、同じProductInfoインスタンスを共有します。
 *
 * @author VendingMachine Team
 * @version 1.0.0
 */

#ifndef VENDING_MACHINE_DOMAIN_INVENTORY_PRODUCTINFO_HPP
#define VENDING_MACHINE_DOMAIN_INVENTORY_PRODUCTINFO_HPP

#include "domain/common/Price.hpp"
#include "domain/inventory/ProductName.hpp"
#include <string>

namespace vending_machine {
namespace domain {

/**
 * @class ProductInfo
 * @brief 商品情報の値オブジェクト
 *
 * 商品の名前と価格を不変のペアとして保持します。
 */
class ProductInfo {
public:
  /**
   * @brief コンストラクタ
   * @param name 商品名
   * @param price 商品価格
   */
  ProductInfo(const ProductName &name, const Price &price);

  /**
   * @brief 商品名を取得
   * @return 商品名
   */
  const ProductName &getName() const;

  /**
   * @brief 商品価格を取得
   * @return 商品価格
   */
  const Price &getPrice() const;

  /**
   * @brief 等価演算子
   * @param other 比較対象
   * @return 名前と価格が等しい場合true
   */
  bool operator==(const ProductInfo &other) const;

  /**
   * @brief 不等価演算子
   * @param other 比較対象
   * @return 名前または価格が等しくない場合true
   */
  bool operator!=(const ProductInfo &other) const;

private:
  ProductName name_; ///< 商品名
  Price price_;      ///< 商品価格
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_INVENTORY_PRODUCTINFO_HPP
