#include "TransactionSession.hpp"
#include <stdexcept>

namespace vending_machine {
namespace domain {

TransactionSession::TransactionSession(const SessionId &session_id)
    : session_id_(session_id), status_(SessionStatus::PRODUCT_SELECTING),
      selected_slot_id_(std::nullopt) {}

const SessionId &TransactionSession::getSessionId() const {
  return session_id_;
}

SessionStatus TransactionSession::getStatus() const { return status_; }

std::optional<SlotId> TransactionSession::getSelectedSlotId() const {
  return selected_slot_id_;
}

void TransactionSession::selectProduct(const SlotId &slot_id) {
  if (selected_slot_id_.has_value()) {
    throw std::domain_error("Product already selected in this session");
  }
  selected_slot_id_ = slot_id;
}

void TransactionSession::markPaymentPending() {
  if (!selected_slot_id_.has_value()) {
    throw std::domain_error("No product selected");
  }
  if (status_ != SessionStatus::PRODUCT_SELECTING) {
    throw std::domain_error("Invalid state transition to PAYMENT_PENDING");
  }
  status_ = SessionStatus::PAYMENT_PENDING;
}

void TransactionSession::markDispensing() {
  if (status_ != SessionStatus::PAYMENT_PENDING) {
    throw std::domain_error("Invalid state transition to DISPENSING");
  }
  status_ = SessionStatus::DISPENSING;
}

void TransactionSession::complete() {
  if (status_ != SessionStatus::DISPENSING) {
    throw std::domain_error("Invalid state transition to COMPLETED");
  }
  status_ = SessionStatus::COMPLETED;
}

void TransactionSession::cancel() { status_ = SessionStatus::CANCELLED; }

bool TransactionSession::isFinished() const {
  return status_ == SessionStatus::COMPLETED ||
         status_ == SessionStatus::CANCELLED;
}

} // namespace domain
} // namespace vending_machine
