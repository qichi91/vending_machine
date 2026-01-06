#include "InMemoryInventoryRepository.hpp"

namespace vending_machine::adapters::outbound {

Inventory InMemoryInventoryRepository::getInventory() {
  return currentInventory_;
}

void InMemoryInventoryRepository::save(const Inventory &inventory) {
  currentInventory_ = inventory;
}

} // namespace vending_machine::adapters::outbound