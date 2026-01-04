#include "EligibleProduct.hpp"

namespace vending_machine {
namespace domain {

EligibleProduct::EligibleProduct(const SlotId &slot_id,
                                 const ProductInfo &product_info)
    : slot_id_(slot_id), product_info_(product_info) {}

const SlotId &EligibleProduct::getSlotId() const { return slot_id_; }

const ProductInfo &EligibleProduct::getProductInfo() const {
  return product_info_;
}

bool EligibleProduct::operator==(const EligibleProduct &other) const {
  return slot_id_ == other.slot_id_ && product_info_ == other.product_info_;
}

bool EligibleProduct::operator!=(const EligibleProduct &other) const {
  return !(*this == other);
}

} // namespace domain
} // namespace vending_machine
