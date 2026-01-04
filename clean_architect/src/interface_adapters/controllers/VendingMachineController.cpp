#include "VendingMachineController.hpp"
#include "domain/common/Money.hpp"
#include "domain/common/Quantity.hpp"
#include "domain/inventory/SlotId.hpp"

namespace vending_machine {
namespace interface_adapters {

VendingMachineController::VendingMachineController(
    usecases::PurchaseWithCashUseCase &purchase_cash_usecase,
    usecases::PurchaseWithEMoneyUseCase &purchase_emoney_usecase,
    usecases::InventoryRefillUseCase &refill_usecase,
    usecases::SalesReportingUseCase &reporting_usecase,
    usecases::CashCollectionUseCase &cash_collection_usecase)
    : purchase_cash_usecase_(purchase_cash_usecase),
      purchase_emoney_usecase_(purchase_emoney_usecase),
      refill_usecase_(refill_usecase), reporting_usecase_(reporting_usecase),
      cash_collection_usecase_(cash_collection_usecase) {}

void VendingMachineController::startCashPurchaseSession() {
  purchase_cash_usecase_.startSession();
}

void VendingMachineController::insertCash(int amount) {
  usecases::dto::InsertCashRequest request{amount};
  purchase_cash_usecase_.insertCash(request);
}

std::vector<usecases::dto::ProductDto>
VendingMachineController::getEligibleProducts() {
  return purchase_cash_usecase_.getEligibleProducts();
}

usecases::dto::PurchaseResponse
VendingMachineController::purchaseWithCash(int slot_id) {
  usecases::dto::PurchaseRequest request{slot_id};
  return purchase_cash_usecase_.selectAndPurchase(request);
}

int VendingMachineController::getBalance() {
  return purchase_cash_usecase_.getBalance();
}

int VendingMachineController::refund() {
  return purchase_cash_usecase_.refund();
}

void VendingMachineController::startEMoneyPurchaseSession() {
  purchase_emoney_usecase_.startSession();
}

std::vector<usecases::dto::ProductDto>
VendingMachineController::getAvailableProductsForEMoney() {
  return purchase_emoney_usecase_.getAvailableProducts();
}

usecases::dto::EMoneyPurchaseResponse
VendingMachineController::purchaseWithEMoney(int slot_id) {
  usecases::dto::EMoneyPurchaseRequest request{slot_id};
  return purchase_emoney_usecase_.selectAndRequestPayment(request);
}

void VendingMachineController::refillInventory(int slot_id, int quantity) {
  refill_usecase_.refillSlot(domain::SlotId(slot_id),
                             domain::Quantity(quantity));
}

int VendingMachineController::collectCash() {
  return cash_collection_usecase_.collectCash().getRawValue();
}

usecases::dto::SalesReportDto VendingMachineController::getSalesReport() {
  auto payment_reports = reporting_usecase_.generatePaymentMethodReport();

  int total = 0;
  int cash = 0;
  int emoney = 0;

  for (const auto &report : payment_reports) {
    int amount = report.total_revenue.getRawValue();
    total += amount;
    if (report.payment_method == domain::PaymentMethodType::CASH) {
      cash += amount;
    } else if (report.payment_method == domain::PaymentMethodType::EMONEY) {
      emoney += amount;
    }
  }

  return {total, cash, emoney};
}

std::vector<usecases::dto::ProductDto>
VendingMachineController::getAllProducts() {
  return purchase_cash_usecase_.getAllProducts();
}

} // namespace interface_adapters
} // namespace vending_machine
