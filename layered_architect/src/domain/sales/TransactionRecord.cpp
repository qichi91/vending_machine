#include "TransactionRecord.hpp"

namespace vending_machine {
namespace domain {

TransactionRecord::TransactionRecord(const SalesId &sales_id,
                                     const SlotId &slot_id, const Price &price,
                                     PaymentMethodType payment_method)
    : TransactionRecord(sales_id, slot_id, price, payment_method,
                        std::chrono::system_clock::now()) {}

TransactionRecord::TransactionRecord(
    const SalesId &sales_id, const SlotId &slot_id, const Price &price,
    PaymentMethodType payment_method,
    std::chrono::system_clock::time_point timestamp)
    : sales_id_(sales_id), slot_id_(slot_id), price_(price),
      payment_method_(payment_method), timestamp_(timestamp) {}

const SalesId &TransactionRecord::getSalesId() const { return sales_id_; }

const SlotId &TransactionRecord::getSlotId() const { return slot_id_; }

const Price &TransactionRecord::getPrice() const { return price_; }

PaymentMethodType TransactionRecord::getPaymentMethod() const {
  return payment_method_;
}

std::chrono::system_clock::time_point TransactionRecord::getTimestamp() const {
  return timestamp_;
}

} // namespace domain
} // namespace vending_machine
