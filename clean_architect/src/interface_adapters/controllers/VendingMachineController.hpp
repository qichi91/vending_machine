#ifndef VENDING_MACHINE_INTERFACE_ADAPTERS_CONTROLLERS_VENDING_MACHINE_CONTROLLER_HPP
#define VENDING_MACHINE_INTERFACE_ADAPTERS_CONTROLLERS_VENDING_MACHINE_CONTROLLER_HPP

#include "usecases/CashCollectionUseCase.hpp"
#include "usecases/InventoryRefillUseCase.hpp"
#include "usecases/PurchaseWithCashUseCase.hpp"
#include "usecases/PurchaseWithEMoneyUseCase.hpp"
#include "usecases/SalesReportingUseCase.hpp"
#include "usecases/dto/PurchaseDTOs.hpp"
#include <string>
#include <vector>

namespace vending_machine {
namespace interface_adapters {

class VendingMachineController {
public:
  VendingMachineController(
      usecases::PurchaseWithCashUseCase &purchase_cash_usecase,
      usecases::PurchaseWithEMoneyUseCase &purchase_emoney_usecase,
      usecases::InventoryRefillUseCase &refill_usecase,
      usecases::SalesReportingUseCase &reporting_usecase,
      usecases::CashCollectionUseCase &cash_collection_usecase);

  // Cash Purchase
  void startCashPurchaseSession();
  void insertCash(int amount);
  std::vector<usecases::dto::ProductDto> getEligibleProducts();
  usecases::dto::PurchaseResponse purchaseWithCash(int slot_id);
  int getBalance();
  int refund();

  // E-Money Purchase
  void startEMoneyPurchaseSession();
  std::vector<usecases::dto::ProductDto> getAvailableProductsForEMoney();
  usecases::dto::EMoneyPurchaseResponse purchaseWithEMoney(int slot_id);

  // Admin / Maintenance
  void refillInventory(int slot_id, int quantity);
  int collectCash();
  usecases::dto::SalesReportDto getSalesReport();

  // Product Info (General)
  std::vector<usecases::dto::ProductDto> getAllProducts();

private:
  usecases::PurchaseWithCashUseCase &purchase_cash_usecase_;
  usecases::PurchaseWithEMoneyUseCase &purchase_emoney_usecase_;
  usecases::InventoryRefillUseCase &refill_usecase_;
  usecases::SalesReportingUseCase &reporting_usecase_;
  usecases::CashCollectionUseCase &cash_collection_usecase_;
};

} // namespace interface_adapters
} // namespace vending_machine

#endif
