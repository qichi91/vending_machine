#pragma once

#include "domain/Inventory.hpp"

namespace vending_machine::ports::outbound {

using namespace domain;

class IInventoryRepository {
public:
  virtual ~IInventoryRepository() = default;

  // 在庫情報を取得する
  virtual Inventory getInventory() = 0;

  // 在庫情報を保存する
  virtual void save(const Inventory &inventory) = 0;
};

} // namespace vending_machine::ports::outbound