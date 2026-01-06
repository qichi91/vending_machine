#include "Product.hpp"

namespace vending_machine::domain {

Product::Product(const std::string &name, const Money &price)
    : name_(name), price_(price) {}

std::string Product::name() const { return name_; }

Money Product::price() const { return price_; }

bool Product::operator==(const Product &other) const {
  return name_ == other.name_ && price_ == other.price_;
}

bool Product::operator!=(const Product &other) const {
  return !(*this == other);
}

bool Product::operator<(const Product &other) const {
  if (name_ != other.name_) {
    return name_ < other.name_;
  }
  return price_ < other.price_;
}

} // namespace vending_machine::domain