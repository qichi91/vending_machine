#include "SalesId.hpp"

namespace vending_machine {
namespace domain {

SalesId::SalesId(int value) : value_(value) {
  if (value <= 0) {
    throw std::invalid_argument("SalesId must be positive");
  }
}

int SalesId::getValue() const { return value_; }

bool SalesId::operator==(const SalesId &other) const {
  return value_ == other.value_;
}

bool SalesId::operator!=(const SalesId &other) const {
  return value_ != other.value_;
}

} // namespace domain
} // namespace vending_machine
