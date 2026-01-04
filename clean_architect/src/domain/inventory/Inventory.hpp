/**
 * @file Inventory.hpp
 * @brief Inventory Aggregate - 在庫集約
 *
 * @details
 * Inventoryは自動販売機内の全スロットを管理するアグリゲートです。
 * 複数のProductSlotを保持し、在庫の管理・更新を提供します。
 *
 * 責務:
 * - 複数のスロットを管理
 * - スロットID単位の在庫操作（販売、補充）
 * - 在庫の完全性を保証
 *
 * @author VendingMachine Team
 * @version 1.0.0
 */

#ifndef VENDING_MACHINE_DOMAIN_INVENTORY_INVENTORY_HPP
#define VENDING_MACHINE_DOMAIN_INVENTORY_INVENTORY_HPP

#include "ProductSlot.hpp"
#include "SlotId.hpp"
#include <map>
#include <memory>
#include <stdexcept>

namespace vending_machine {
namespace domain {

/**
 * @class Inventory
 * @brief 自動販売機の在庫を管理するアグリゲートルート
 *
 * 複数のProductSlotを保持し、スロット単位での在庫操作をサポートします。
 */
class Inventory {
public:
  /**
   * @brief コンストラクタ
   */
  Inventory();

  /**
   * @brief スロットを追加
   * @param slot 追加するProductSlot
   * @throw std::invalid_argument 同じSlotIdが既に存在する場合
   */
  void addSlot(const ProductSlot &slot);

  /**
   * @brief スロットIDでスロットを取得
   * @param slot_id 取得するスロットID
   * @return ProductSlotへの参照
   * @throw std::invalid_argument スロットが存在しない場合
   */
  ProductSlot &getSlot(const SlotId &slot_id);

  /**
   * @brief スロットIDでスロットを取得（const版）
   * @param slot_id 取得するスロットID
   * @return ProductSlotへのconst参照
   * @throw std::invalid_argument スロットが存在しない場合
   */
  const ProductSlot &getSlot(const SlotId &slot_id) const;

  /**
   * @brief 指定のスロットから商品を1個販売する
   * @param slot_id スロットID
   * @throw std::invalid_argument スロットが存在しない場合
   * @throw std::domain_error スロットの在庫が0の場合
   */
  void dispense(const SlotId &slot_id);

  /**
   * @brief 指定のスロットに在庫を補充
   * @param slot_id スロットID
   * @param amount 補充する数量
   * @throw std::invalid_argument スロットが存在しない場合
   * @throw std::domain_error 補充後の在庫が最大収容数を超える場合
   */
  void refill(const SlotId &slot_id, const Quantity &amount);

  /**
   * @brief すべてのスロットを取得
   * @return スロットIDとProductSlotの共有ポインタのマップ
   *
   * @note ドメインサービスでは複数の集約を横断した処理が必要であり、
   *       完全なカプセル化よりもシンプルさとパフォーマンスを優先します。
   */
  const std::map<SlotId, std::shared_ptr<ProductSlot>> &getAllSlots() const;

private:
  std::map<SlotId, std::shared_ptr<ProductSlot>>
      slots_; ///< SlotId => ProductSlotへのポインタのマップ
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_INVENTORY_INVENTORY_HPP
