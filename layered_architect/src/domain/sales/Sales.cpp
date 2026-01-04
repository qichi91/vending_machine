#include "Sales.hpp"
#include <stdexcept>

namespace vending_machine {
namespace domain {

Sales::Sales(const SalesId &sales_id)
    : sales_id_(sales_id), mode_(Mode::NORMAL), current_session_(nullptr) {}

const SalesId &Sales::getId() const { return sales_id_; }

Mode Sales::getMode() const { return mode_; }

const TransactionSession *Sales::getCurrentSession() const {
  return current_session_.get();
}

std::optional<SalesId> Sales::getCurrentSessionSalesId() const {
  if (current_session_ == nullptr) {
    return std::nullopt;
  }
  return sales_id_;
}

void Sales::startSession(const SessionId &session_id) {
  if (mode_ == Mode::MAINTENANCE) {
    throw std::domain_error("Cannot start session in maintenance mode");
  }
  if (current_session_ != nullptr) {
    throw std::domain_error("Session already exists");
  }
  current_session_ = std::make_unique<TransactionSession>(session_id);
}

void Sales::selectProduct(const SlotId &slot_id) {
  if (current_session_ == nullptr) {
    throw std::domain_error("No active session");
  }
  current_session_->selectProduct(slot_id);
}

void Sales::markPaymentPending() {
  if (current_session_ == nullptr) {
    throw std::domain_error("No active session");
  }
  current_session_->markPaymentPending();
}

void Sales::markDispensing() {
  if (current_session_ == nullptr) {
    throw std::domain_error("No active session");
  }
  current_session_->markDispensing();
}

void Sales::completeTransaction() {
  if (current_session_ == nullptr) {
    throw std::domain_error("No active session");
  }
  current_session_->complete();
  current_session_.reset();
}

void Sales::cancelTransaction() {
  if (current_session_ == nullptr) {
    throw std::domain_error("No active session");
  }
  current_session_->cancel();
  current_session_.reset();
}

void Sales::startMaintenance() {
  if (current_session_ != nullptr && !current_session_->isFinished()) {
    throw std::domain_error("Cannot start maintenance with active session");
  }
  mode_ = Mode::MAINTENANCE;
}

void Sales::endMaintenance() { mode_ = Mode::NORMAL; }

} // namespace domain
} // namespace vending_machine
