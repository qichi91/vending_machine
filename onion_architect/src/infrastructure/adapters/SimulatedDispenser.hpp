/**
 * @file SimulatedDispenser.hpp
 * @brief シミュレーション用ディスペンサー
 */

#ifndef VENDING_MACHINE_INFRASTRUCTURE_SIMULATED_DISPENSER_HPP
#define VENDING_MACHINE_INFRASTRUCTURE_SIMULATED_DISPENSER_HPP

#include "domain/interfaces/IDispenser.hpp"
#include "domain/inventory/ProductInfo.hpp"
#include <iostream>

namespace vending_machine {
namespace infrastructure {

class SimulatedDispenser : public domain::IDispenser {
public:
  bool canDispense(const domain::ProductInfo &product) const override {
    return true; // 常に排出可能
  }

  void dispense(const domain::ProductInfo &product) override {
    std::cout << "(スロットから " << product.getName().getValue()
              << " を排出)\n";
  }
};

} // namespace infrastructure
} // namespace vending_machine

#endif // VENDING_MACHINE_INFRASTRUCTURE_SIMULATED_DISPENSER_HPP
