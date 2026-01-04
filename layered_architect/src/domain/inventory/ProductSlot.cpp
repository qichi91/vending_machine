/**
 * @file ProductSlot.cpp
 * @brief ProductSlot Entity の実装
 */

#include "ProductSlot.hpp"

namespace vending_machine {
namespace domain {

ProductSlot::ProductSlot(SlotId id, const ProductInfo &info,
                         const Quantity &stock)
    : id_(id), info_(info), stock_(stock) {}

const SlotId &ProductSlot::getSlotId() const { return id_; }

const ProductInfo &ProductSlot::getProductInfo() const { return info_; }

const Quantity &ProductSlot::getStock() const { return stock_; }

bool ProductSlot::isAvailable() const { return !stock_.isZero(); }

void ProductSlot::dispense() {
  if (!isAvailable()) {
    throw std::domain_error("Cannot dispense from empty slot");
  }
  stock_ = stock_.decrease(1);
}

void ProductSlot::refill(const Quantity &amount) {
  stock_ = stock_.increase(amount.getValue());
}

} // namespace domain
} // namespace vending_machine
