#include "SessionId.hpp"

namespace vending_machine {
namespace domain {

SessionId::SessionId(int value) : value_(value) {
  if (value <= 0) {
    throw std::invalid_argument("SessionId must be positive");
  }
}

int SessionId::getValue() const { return value_; }

bool SessionId::operator==(const SessionId &other) const {
  return value_ == other.value_;
}

bool SessionId::operator!=(const SessionId &other) const {
  return value_ != other.value_;
}

} // namespace domain
} // namespace vending_machine
