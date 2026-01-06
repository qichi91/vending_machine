#pragma once

#include "domain/VendingMachine.hpp"
#include "ports/inbound/IMaintenanceService.hpp"
#include "ports/inbound/IVendingMachineService.hpp"
#include "ports/outbound/IInventoryRepository.hpp"
#include "ports/outbound/IPaymentGateway.hpp"

namespace vending_machine::application {

using namespace ports::inbound;
using namespace ports::outbound;
using namespace domain;

class VendingMachineService : public IVendingMachineService,
                              public IMaintenanceService {
public:
  // TODO: PaymentGateway is optional or required? Making it required for now.
  // In real world, maybe set via setter or pass in constructor.
  VendingMachineService(IInventoryRepository &repository,
                        IPaymentGateway &paymentGateway);

  // IVendingMachineService
  void insertMoney(const Money &money) override;
  Product selectProduct(const std::string &productName) override;
  int getCurrentAmount() const override;
  std::vector<Money> getRefund() override;

  // New feature: pay with e-money
  // This was not in IVendingMachineService interface, assuming we add it or
  // cast? User requested "implemented like others". I should add this method to
  // IVendingMachineService. Let's modify IVendingMachineService first. For TDD,
  // I will modify VendingMachineService first then interface.
  Product purchaseWithElectronicMoney(const std::string &productName);

  // IMaintenanceService
  void addStock(const std::string &name, int price, int count) override;
  int getTotalSales() const override;

private:
  IInventoryRepository &repository_;
  IPaymentGateway &paymentGateway_;
  VendingMachine vendingMachine_;
};

} // namespace vending_machine::application