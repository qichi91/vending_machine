#include "application/VendingMachineApplication.hpp"
#include "domain/inventory/ProductSlot.hpp"

namespace vending_machine {
namespace application {

VendingMachineApplication::VendingMachineApplication(
    domain::ICoinMech &coin_mech, domain::IDispenser &dispenser,
    domain::IPaymentGateway &payment_gateway,
    domain::ITransactionHistoryRepository &transaction_history)
    : sales_(domain::SalesId(1)), coin_mech_(coin_mech), dispenser_(dispenser),
      payment_gateway_(payment_gateway),
      transaction_history_(transaction_history) {

  // ユースケースを初期化
  purchase_with_cash_usecase_ = std::make_unique<PurchaseWithCashUseCase>(
      inventory_, wallet_, sales_, coin_mech_, dispenser_,
      transaction_history_);

  purchase_with_emoney_usecase_ = std::make_unique<PurchaseWithEMoneyUseCase>(
      inventory_, wallet_, sales_, payment_gateway_, dispenser_,
      transaction_history_);

  inventory_refill_usecase_ =
      std::make_unique<InventoryRefillUseCase>(inventory_);

  cash_collection_usecase_ =
      std::make_unique<CashCollectionUseCase>(transaction_history_);

  sales_reporting_usecase_ =
      std::make_unique<SalesReportingUseCase>(transaction_history_);
}

void VendingMachineApplication::initializeInventory() {
  // スロット1: コーラ (120円, 10個)
  domain::ProductSlot slot1(
      domain::SlotId(1),
      domain::ProductInfo(domain::ProductName("コーラ"), domain::Price(120)),
      domain::Quantity(10));
  inventory_.addSlot(slot1);

  // スロット2: お茶 (150円, 10個)
  domain::ProductSlot slot2(
      domain::SlotId(2),
      domain::ProductInfo(domain::ProductName("お茶"), domain::Price(150)),
      domain::Quantity(10));
  inventory_.addSlot(slot2);

  // スロット3: 水 (100円, 10個)
  domain::ProductSlot slot3(
      domain::SlotId(3),
      domain::ProductInfo(domain::ProductName("水"), domain::Price(100)),
      domain::Quantity(10));
  inventory_.addSlot(slot3);

  // スロット4: コーヒー (130円, 10個)
  domain::ProductSlot slot4(
      domain::SlotId(4),
      domain::ProductInfo(domain::ProductName("コーヒー"), domain::Price(130)),
      domain::Quantity(10));
  inventory_.addSlot(slot4);
}

} // namespace application
} // namespace vending_machine
