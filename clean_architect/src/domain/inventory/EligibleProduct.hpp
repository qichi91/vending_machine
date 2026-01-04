#ifndef VENDING_MACHINE_DOMAIN_ELIGIBLE_PRODUCT_HPP
#define VENDING_MACHINE_DOMAIN_ELIGIBLE_PRODUCT_HPP

#include "ProductInfo.hpp"
#include "SlotId.hpp"

namespace vending_machine {
namespace domain {

/**
 * @class EligibleProduct
 * @brief 購入可能な商品を表す値オブジェクト
 *
 * 商品情報に加えて、現在の残高・在庫・釣銭状況に照らして
 * 「その利用者が購入可能である」という論理的な適格性を
 * 付与されたオブジェクトです。
 */
class EligibleProduct {
public:
  /**
   * @brief コンストラクタ
   * @param slot_id スロットID
   * @param product_info 商品情報
   */
  EligibleProduct(const SlotId &slot_id, const ProductInfo &product_info);

  /**
   * @brief スロットIDを取得
   * @return スロットID
   */
  const SlotId &getSlotId() const;

  /**
   * @brief 商品情報を取得
   * @return 商品情報
   */
  const ProductInfo &getProductInfo() const;

  /**
   * @brief 等価性を比較
   * @param other 比較対象
   * @return 等しい場合true
   */
  bool operator==(const EligibleProduct &other) const;

  /**
   * @brief 不等価性を比較
   * @param other 比較対象
   * @return 等しくない場合true
   */
  bool operator!=(const EligibleProduct &other) const;

private:
  SlotId slot_id_;           ///< スロットID
  ProductInfo product_info_; ///< 商品情報
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_ELIGIBLE_PRODUCT_HPP
