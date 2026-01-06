#include "VendingMachine.hpp"
#include <stdexcept>

namespace vending_machine::domain {

VendingMachine::VendingMachine() : currentAmount_(0), totalSales_(0) {}

void VendingMachine::addStock(const Product &product, int count) {
  inventory_.add(product, count);
}

void VendingMachine::setInventory(const Inventory &inventory) {
  inventory_ = inventory;
}

void VendingMachine::insertMoney(const Money &money) {
  if (!money.isValidDenomination()) {
    throw std::invalid_argument("Invalid money denomination: " +
                                std::to_string(money.amount()));
  }
  currentAmount_ += money.amount();
}

Product VendingMachine::purchase(const std::string &productName) {
  const Product *product = inventory_.findProductByName(productName);

  if (!product) {
    throw std::runtime_error("Product not found: " + productName);
  }

  if (!inventory_.hasStock(*product)) {
    throw std::runtime_error("Out of stock: " + productName);
  }

  if (currentAmount_ < product->price().amount()) {
    throw std::runtime_error("Insufficient funds for: " + productName);
  }

  // Purchase process
  currentAmount_ -= product->price().amount();
  totalSales_ += product->price().amount();
  inventory_.reduce(*product);

  return *product;
}

Product
VendingMachine::purchaseViaElectronicMoney(const std::string &productName) {
  const Product *product = inventory_.findProductByName(productName);

  if (!product) {
    throw std::runtime_error("Product not found: " + productName);
  }

  if (!inventory_.hasStock(*product)) {
    throw std::runtime_error("Out of stock: " + productName);
  }

  // 電子マネー決済の場合は投入金額のチェックを行わない
  // 承認済みであることを前提とする

  totalSales_ += product->price().amount();
  inventory_.reduce(*product);

  return *product;
}

int VendingMachine::getSales() const { return totalSales_; }

const Product *
VendingMachine::findProductByName(const std::string &name) const {
  return inventory_.findProductByName(name);
}

int VendingMachine::currentAmount() const { return currentAmount_; }

int VendingMachine::refund() {
  int amount = currentAmount_;
  currentAmount_ = 0;
  return amount;
}

const Inventory &VendingMachine::inventory() const { return inventory_; }

// purchaseの実装はInventoryの拡張後に記述する

} // namespace vending_machine::domain