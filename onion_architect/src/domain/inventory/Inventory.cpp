/**
 * @file Inventory.cpp
 * @brief Inventory Aggregate の実装
 */

#include "Inventory.hpp"

namespace vending_machine {
namespace domain {

Inventory::Inventory() {}

void Inventory::addSlot(const ProductSlot &slot) {
  const SlotId &slot_id = slot.getSlotId();

  if (slots_.find(slot_id) != slots_.end()) {
    throw std::invalid_argument("SlotId already exists in inventory");
  }

  slots_[slot_id] = std::make_shared<ProductSlot>(slot);
}

ProductSlot &Inventory::getSlot(const SlotId &slot_id) {
  auto it = slots_.find(slot_id);
  if (it == slots_.end()) {
    throw std::invalid_argument("Slot not found in inventory");
  }

  return *it->second;
}

const ProductSlot &Inventory::getSlot(const SlotId &slot_id) const {
  auto it = slots_.find(slot_id);
  if (it == slots_.end()) {
    throw std::invalid_argument("Slot not found in inventory");
  }

  return *it->second;
}

void Inventory::dispense(const SlotId &slot_id) {
  ProductSlot &slot = getSlot(slot_id);
  slot.dispense();
}

void Inventory::refill(const SlotId &slot_id, const Quantity &amount) {
  ProductSlot &slot = getSlot(slot_id);
  slot.refill(amount);
}

const std::map<SlotId, std::shared_ptr<ProductSlot>> &
Inventory::getAllSlots() const {
  return slots_;
}

} // namespace domain
} // namespace vending_machine
