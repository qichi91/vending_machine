/**
 * @file ProductName.cpp
 * @brief ProductName Value Object の実装
 */

#include "ProductName.hpp"

namespace vending_machine {
namespace domain {

ProductName::ProductName(const std::string &name) : name_(name) {
  if (name.empty()) {
    throw std::invalid_argument("Product name cannot be empty");
  }
}

const std::string &ProductName::getValue() const { return name_; }

bool ProductName::operator==(const ProductName &other) const {
  return name_ == other.name_;
}

bool ProductName::operator!=(const ProductName &other) const {
  return !(*this == other);
}

} // namespace domain
} // namespace vending_machine
