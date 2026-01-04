/**
 * @file SimulatedCoinMech.hpp
 * @brief シミュレーション用硬貨処理機
 */

#ifndef VENDING_MACHINE_INFRASTRUCTURE_SIMULATED_COIN_MECH_HPP
#define VENDING_MACHINE_INFRASTRUCTURE_SIMULATED_COIN_MECH_HPP

#include "domain/interfaces/ICoinMech.hpp"
#include <iostream>

namespace vending_machine {
namespace interface_adapters {

class SimulatedCoinMech : public domain::ICoinMech {
public:
  bool canMakeChange(const domain::Money &amount) const override {
    return true; // 常に釣り銭可能
  }

  void dispense(const domain::Money &amount) override {
    std::cout << "(おつり " << amount.getRawValue() << "円を排出)\n";
  }
};

} // namespace interface_adapters
} // namespace vending_machine

#endif // VENDING_MACHINE_INFRASTRUCTURE_SIMULATED_COIN_MECH_HPP
