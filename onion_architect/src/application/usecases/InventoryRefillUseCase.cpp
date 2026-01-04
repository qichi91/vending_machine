#include "InventoryRefillUseCase.hpp"
#include "domain/common/Quantity.hpp"
#include "domain/inventory/Inventory.hpp"
#include "domain/inventory/SlotId.hpp"

namespace vending_machine {
namespace application {

InventoryRefillUseCase::InventoryRefillUseCase(domain::Inventory &inventory)
    : inventory_(inventory) {}

void InventoryRefillUseCase::refillSlot(const domain::SlotId &slot_id,
                                        const domain::Quantity &quantity) {
  inventory_.refill(slot_id, quantity);
}

void InventoryRefillUseCase::refillMultiple(
    const std::vector<std::pair<domain::SlotId, domain::Quantity>> &refills) {
  for (const auto &[slot_id, quantity] : refills) {
    refillSlot(slot_id, quantity);
  }
}

domain::Quantity
InventoryRefillUseCase::getSlotQuantity(const domain::SlotId &slot_id) const {
  return inventory_.getSlot(slot_id).getStock();
}

} // namespace application
} // namespace vending_machine
