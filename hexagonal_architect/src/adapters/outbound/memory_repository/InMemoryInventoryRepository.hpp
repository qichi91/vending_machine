#pragma once

#include "ports/outbound/IInventoryRepository.hpp"
#include <map>

namespace vending_machine::adapters::outbound {

using namespace ports::outbound;
using namespace domain;

class InMemoryInventoryRepository : public IInventoryRepository {
public:
  Inventory getInventory() override;
  void save(const Inventory &inventory) override;

private:
  Inventory currentInventory_;
};

} // namespace vending_machine::adapters::outbound