/**
 * @file ProductInfo.cpp
 * @brief ProductInfo Value Object の実装
 */

#include "ProductInfo.hpp"

namespace vending_machine {
namespace domain {

ProductInfo::ProductInfo(const ProductName &name, const Price &price)
    : name_(name), price_(price) {}

const ProductName &ProductInfo::getName() const { return name_; }

const Price &ProductInfo::getPrice() const { return price_; }

bool ProductInfo::operator==(const ProductInfo &other) const {
  return name_ == other.name_ && price_ == other.price_;
}

bool ProductInfo::operator!=(const ProductInfo &other) const {
  return !(*this == other);
}

} // namespace domain
} // namespace vending_machine
