#pragma once

#include "domain/Money.hpp"
#include "domain/Product.hpp"
#include <string>

namespace vending_machine::ports::inbound {

class IMaintenanceService {
public:
  virtual ~IMaintenanceService() = default;

  // 商品を補充する
  virtual void addStock(const std::string &name, int price, int count) = 0;

  // 総売上金額を確認する
  virtual int getTotalSales() const = 0;
};

} // namespace vending_machine::ports::inbound