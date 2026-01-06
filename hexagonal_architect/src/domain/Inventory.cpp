#include "Inventory.hpp"
#include <stdexcept>

namespace vending_machine::domain {

void Inventory::add(const Product &product, int count) {
  if (count < 0) {
    throw std::invalid_argument("Count must be non-negative");
  }
  stocks_[product] += count;
}

void Inventory::reduce(const Product &product) {
  auto it = stocks_.find(product);
  if (it == stocks_.end() || it->second <= 0) {
    throw std::runtime_error("Out of stock: " + product.name());
  }
  it->second--;
}

bool Inventory::hasStock(const Product &product) const {
  auto it = stocks_.find(product);
  return it != stocks_.end() && it->second > 0;
}

int Inventory::getCount(const Product &product) const {
  auto it = stocks_.find(product);
  if (it == stocks_.end()) {
    return 0;
  }
  return it->second;
}

const Product *Inventory::findProductByName(const std::string &name) const {
  for (const auto &pair : stocks_) {
    if (pair.first.name() == name) {
      return &pair.first;
    }
  }
  return nullptr;
}

} // namespace vending_machine::domain