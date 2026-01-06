#include "ConsoleAdapter.hpp"
#include <sstream>
#include <vector>

namespace vending_machine::adapters::inbound {

using namespace domain;

ConsoleAdapter::ConsoleAdapter(ports::inbound::IVendingMachineService &service,
                               ports::inbound::IMaintenanceService &maintenance,
                               std::istream &in, std::ostream &out)
    : service_(service), maintenance_(maintenance), in_(in), out_(out) {}

void ConsoleAdapter::run() {
  out_ << "Hexagonal Vending Machine Started.\n";
  out_ << "Commands: insert <amount>, select <product>, emoney <product>, "
          "restock <name> <price> <count>, sales, refund, exit\n";

  std::string line;
  while (std::getline(in_, line)) {
    if (line == "exit")
      break;
    try {
      processCommand(line);
    } catch (const std::exception &e) {
      out_ << "Error: " << e.what() << "\n";
    }
  }
}

void ConsoleAdapter::processCommand(const std::string &line) {
  std::stringstream ss(line);
  std::string command;
  ss >> command;

  if (command == "insert") {
    handleInsert(ss);
  } else if (command == "select") {
    handleSelect(ss);
  } else if (command == "emoney") {
    handlePayEmoney(ss);
  } else if (command == "restock") {
    handleAddStock(ss);
  } else if (command == "sales") {
    out_ << "Total Sales: " << maintenance_.getTotalSales() << "\n";
  } else if (command == "refund") {
    auto coins = service_.getRefund();
    out_ << "Refunded: ";
    for (const auto &c : coins) {
      out_ << c.amount() << " ";
    }
    out_ << "\n";
  } else {
    out_ << "Unknown command\n";
  }

  // Show current status
  out_ << "Current Amount: " << service_.getCurrentAmount() << "\n";
}

void ConsoleAdapter::handleInsert(std::stringstream &ss) {
  int amount;
  if (ss >> amount) {
    service_.insertMoney(Money(amount));
    out_ << "Inserted: " << amount << "\n";
  } else {
    out_ << "Invalid amount\n";
  }
}

void ConsoleAdapter::handleSelect(std::stringstream &ss) {
  std::string productName;
  ss >> productName;
  if (!productName.empty()) {
    Product p = service_.selectProduct(productName);
    out_ << "Dispensed: " << p.name() << "\n";
  } else {
    out_ << "Invalid product name\n";
  }
}

void ConsoleAdapter::handlePayEmoney(std::stringstream &ss) {
  std::string productName;
  ss >> productName;
  if (!productName.empty()) {
    Product p = service_.purchaseWithElectronicMoney(productName);
    out_ << "Dispensed (E-Money): " << p.name() << "\n";
  } else {
    out_ << "Invalid product name\n";
  }
}

void ConsoleAdapter::handleAddStock(std::stringstream &ss) {
  std::string name;
  int price, count;
  if (ss >> name >> price >> count) {
    maintenance_.addStock(name, price, count);
    out_ << "Restocked: " << name << "\n";
  } else {
    out_ << "Invalid arguments. Usage: restock <name> <price> <count>\n";
  }
}

} // namespace vending_machine::adapters::inbound