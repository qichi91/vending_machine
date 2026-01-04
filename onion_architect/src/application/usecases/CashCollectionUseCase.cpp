#include "application/usecases/CashCollectionUseCase.hpp"

namespace vending_machine {
namespace application {

CashCollectionUseCase::CashCollectionUseCase(
    domain::ITransactionHistoryRepository &transaction_history)
    : transaction_history_(transaction_history) {}

domain::Money CashCollectionUseCase::getTotalRevenue() const {
  return transaction_history_.getTotalRevenue();
}

domain::Money CashCollectionUseCase::collectCash() {
  domain::Money total = transaction_history_.getTotalRevenue();
  transaction_history_.clear();
  return total;
}

} // namespace application
} // namespace vending_machine
