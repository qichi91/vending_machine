#ifndef VENDING_MACHINE_APPLICATION_USECASES_INVENTORYREFILLUSECASE_HPP
#define VENDING_MACHINE_APPLICATION_USECASES_INVENTORYREFILLUSECASE_HPP

#include "domain/common/Quantity.hpp"
#include "domain/inventory/SlotId.hpp"
#include <vector>

namespace vending_machine {
namespace domain {
class Inventory;
} // namespace domain

namespace application {

/**
 * @class InventoryRefillUseCase
 * @brief 在庫補充ユースケース
 *
 * メンテナンス時に商品を補充するための機能を提供します。
 */
class InventoryRefillUseCase {
public:
  /**
   * @brief コンストラクタ
   * @param inventory 在庫集約
   */
  explicit InventoryRefillUseCase(domain::Inventory &inventory);

  /**
   * @brief 指定スロットを補充
   * @param slot_id スロットID
   * @param quantity 補充数量
   */
  void refillSlot(const domain::SlotId &slot_id,
                  const domain::Quantity &quantity);

  /**
   * @brief 複数スロットを一括補充
   * @param refills 補充情報のリスト（{SlotId, Quantity}）
   */
  void refillMultiple(
      const std::vector<std::pair<domain::SlotId, domain::Quantity>> &refills);

  /**
   * @brief 指定スロットの在庫数を取得
   * @param slot_id スロットID
   * @return 在庫数
   */
  domain::Quantity getSlotQuantity(const domain::SlotId &slot_id) const;

private:
  domain::Inventory &inventory_;
};

} // namespace application
} // namespace vending_machine

#endif // VENDING_MACHINE_APPLICATION_USECASES_INVENTORYREFILLUSECASE_HPP
