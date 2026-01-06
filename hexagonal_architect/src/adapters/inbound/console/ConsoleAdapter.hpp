#pragma once

#include "ports/inbound/IMaintenanceService.hpp" // Added
#include "ports/inbound/IVendingMachineService.hpp"
#include <iostream>
#include <string>

namespace vending_machine::adapters::inbound {

class ConsoleAdapter {
public:
  // 依存性注入: ServiceとIOストリーム
  // Serviceは
  // VendingMachineService一つだが、Interface分離の原則的には別々に受け取るのが正しい。
  // ここではVendingMachineServiceが両方を実装しているので、dynamic_castで対応するか、
  // あるいはコンストラクタで両方受け取る。
  // 今回は簡単のため、IVendingMachineServiceをメインとして、IMaintenanceServiceも受け取るように変更。
  ConsoleAdapter(ports::inbound::IVendingMachineService &service,
                 ports::inbound::IMaintenanceService &maintenance, // Added
                 std::istream &in = std::cin, std::ostream &out = std::cout);

  void run();

private:
  ports::inbound::IVendingMachineService &service_;
  ports::inbound::IMaintenanceService &maintenance_; // Added
  std::istream &in_;
  std::ostream &out_;

  void processCommand(const std::string &line);
  void handleInsert(std::stringstream &ss);
  void handleSelect(std::stringstream &ss);
  void handlePayEmoney(std::stringstream &ss); // 新規
  void handleAddStock(std::stringstream &ss);  // 新規
};

} // namespace vending_machine::adapters::inbound