/**
 * @file Price.cpp
 * @brief Price Value Object の実装
 */

#include "Price.hpp"

namespace vending_machine {
namespace domain {

Price::Price(int amount) : amount_(amount) {
  if (amount < 0) {
    throw std::invalid_argument("Price cannot be negative");
  }
}

int Price::getRawValue() const { return amount_; }

bool Price::operator==(const Price &other) const {
  return amount_ == other.amount_;
}

bool Price::operator!=(const Price &other) const {
  return amount_ != other.amount_;
}

bool Price::operator<(const Price &other) const {
  return amount_ < other.amount_;
}

bool Price::operator<=(const Price &other) const {
  return amount_ <= other.amount_;
}

bool Price::operator>(const Price &other) const {
  return amount_ > other.amount_;
}

bool Price::operator>=(const Price &other) const {
  return amount_ >= other.amount_;
}

} // namespace domain
} // namespace vending_machine
