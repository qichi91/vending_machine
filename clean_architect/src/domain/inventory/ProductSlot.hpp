/**
 * @file ProductSlot.hpp
 * @brief ProductSlot Entity - スロットの在庫管理
 *
 * @details
 * ProductSlotは自動販売機の特定のスロットにおける商品と在庫を管理するエンティティです。
 * スロットID、商品情報、現在の在庫数を保持し、売却と補充の操作をサポートします。
 *
 * @author VendingMachine Team
 * @version 1.0.0
 */

#ifndef VENDING_MACHINE_DOMAIN_INVENTORY_PRODUCTSLOT_HPP
#define VENDING_MACHINE_DOMAIN_INVENTORY_PRODUCTSLOT_HPP

#include "domain/common/Quantity.hpp"
#include "domain/inventory/ProductInfo.hpp"
#include "domain/inventory/SlotId.hpp"

namespace vending_machine {
namespace domain {

/**
 * @class ProductSlot
 * @brief スロット内の商品在庫を管理するエンティティ
 *
 * 特定のスロットに割り当てられた商品とその在庫数を管理します。
 */
class ProductSlot {
public:
  /**
   * @brief コンストラクタ
   * @param id このスロットのID
   * @param info このスロットに割り当てられた商品情報
   * @param stock 現在の在庫数
   */
  ProductSlot(SlotId id, const ProductInfo &info, const Quantity &stock);

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
   * @brief 現在の在庫数を取得
   * @return 在庫数
   */
  const Quantity &getStock() const;

  /**
   * @brief 在庫が存在するかどうかを判定
   * @return 在庫が1個以上ある場合true
   */
  bool isAvailable() const;

  /**
   * @brief 商品を1個排出する（販売）
   * @throw std::domain_error 在庫が0の場合
   */
  void dispense();

  /**
   * @brief 在庫を補充する
   * @param amount 補充する数量
   * @throw std::domain_error 補充後の在庫が最大収容数を超える場合
   */
  void refill(const Quantity &amount);

private:
  SlotId id_;        ///< スロットID
  ProductInfo info_; ///< 商品情報
  Quantity stock_;   ///< 現在の在庫数
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_INVENTORY_PRODUCTSLOT_HPP
