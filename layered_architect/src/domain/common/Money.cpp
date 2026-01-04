/**
 * @file Money.cpp
 * @brief Money Value Object の実装
 */

#include "Money.hpp"

namespace vending_machine {
namespace domain {

Money::Money(int amount) : amount_(amount) {
  if (amount < 0) {
    throw std::invalid_argument("Money amount cannot be negative");
  }
}

int Money::getRawValue() const { return amount_; }

bool Money::isZero() const { return amount_ == 0; }

Money Money::operator+(const Money &other) const {
  return Money(amount_ + other.amount_);
}

Money Money::operator-(const Money &other) const {
  if (amount_ < other.amount_) {
    throw std::domain_error("Subtraction would result in negative money");
  }
  return Money(amount_ - other.amount_);
}

bool Money::operator==(const Money &other) const {
  return amount_ == other.amount_;
}

bool Money::operator!=(const Money &other) const {
  return amount_ != other.amount_;
}

bool Money::operator<(const Money &other) const {
  return amount_ < other.amount_;
}

bool Money::operator<=(const Money &other) const {
  return amount_ <= other.amount_;
}

bool Money::operator>(const Money &other) const {
  return amount_ > other.amount_;
}

bool Money::operator>=(const Money &other) const {
  return amount_ >= other.amount_;
}

} // namespace domain
} // namespace vending_machine
