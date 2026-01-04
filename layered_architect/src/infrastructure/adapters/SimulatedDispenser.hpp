/**
 * @file SimulatedDispenser.hpp
 * @brief シミュレーション用ディスペンサー
 */

#ifndef VENDING_MACHINE_INFRASTRUCTURE_SIMULATED_DISPENSER_HPP
#define VENDING_MACHINE_INFRASTRUCTURE_SIMULATED_DISPENSER_HPP

#include "domain/interfaces/IDispenser.hpp"
#include <iostream>

namespace vending_machine {
namespace infrastructure {

class SimulatedDispenser : public domain::IDispenser {
public:
  void dispense(const domain::SlotId &slot_id) override {
    std::cout << "(スロット " << slot_id.getValue() << " から商品を排出)\n";
  }
};

} // namespace infrastructure
} // namespace vending_machine

#endif // VENDING_MACHINE_INFRASTRUCTURE_SIMULATED_DISPENSER_HPP
