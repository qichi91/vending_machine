/**
 * @file Quantity.cpp
 * @brief Quantity Value Object の実装
 */

#include "Quantity.hpp"

namespace vending_machine {
namespace domain {

Quantity::Quantity(int value) : value_(value) {
  if (value < 0) {
    throw std::invalid_argument("Quantity cannot be negative");
  }
  if (value > MAX_CAPACITY) {
    throw std::invalid_argument("Quantity cannot exceed maximum capacity");
  }
}

int Quantity::getValue() const { return value_; }

bool Quantity::isZero() const { return value_ == 0; }

Quantity Quantity::increase(int amount) const {
  if (amount < 0) {
    throw std::invalid_argument("Cannot increase by negative amount");
  }

  int newValue = value_ + amount;
  if (newValue > MAX_CAPACITY) {
    throw std::domain_error("Increase would exceed maximum capacity");
  }

  return Quantity(newValue);
}

Quantity Quantity::decrease(int amount) const {
  if (amount < 0) {
    throw std::invalid_argument("Cannot decrease by negative amount");
  }

  if (value_ < amount) {
    throw std::domain_error("Decrease would result in negative quantity");
  }

  return Quantity(value_ - amount);
}

bool Quantity::operator==(const Quantity &other) const {
  return value_ == other.value_;
}

bool Quantity::operator!=(const Quantity &other) const {
  return value_ != other.value_;
}

bool Quantity::operator<(const Quantity &other) const {
  return value_ < other.value_;
}

bool Quantity::operator>(const Quantity &other) const {
  return value_ > other.value_;
}

} // namespace domain
} // namespace vending_machine
