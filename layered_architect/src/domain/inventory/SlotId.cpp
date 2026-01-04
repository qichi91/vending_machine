/**
 * @file SlotId.cpp
 * @brief SlotId Value Object の実装
 */

#include "SlotId.hpp"

namespace vending_machine {
namespace domain {

SlotId::SlotId(int value) : value_(value) {
  if (value <= 0) {
    throw std::invalid_argument("SlotId must be greater than 0");
  }
}

int SlotId::getValue() const { return value_; }

bool SlotId::operator==(const SlotId &other) const {
  return value_ == other.value_;
}

bool SlotId::operator!=(const SlotId &other) const {
  return value_ != other.value_;
}

bool SlotId::operator<(const SlotId &other) const {
  return value_ < other.value_;
}

bool SlotId::operator>(const SlotId &other) const {
  return value_ > other.value_;
}

} // namespace domain
} // namespace vending_machine
