#include "adapters/inbound/console/ConsoleAdapter.hpp"
#include "adapters/outbound/MockPaymentGateway.hpp"
#include "adapters/outbound/memory_repository/InMemoryInventoryRepository.hpp"
#include "application/VendingMachineService.hpp"
#include "domain/Money.hpp"
#include "domain/Product.hpp"

using namespace vending_machine::domain;
using namespace vending_machine::application;
using namespace vending_machine::adapters::inbound;
using namespace vending_machine::adapters::outbound;

int main() {
  // 1. Prepare Dependencies (Driven Adapters)
  InMemoryInventoryRepository repository;
  MockPaymentGateway paymentGateway;

  // 2. Initialize Data (normally this would come from a real DB)
  Inventory initialInventory;
  initialInventory.add(Product("Cola", Money(100)), 5);
  initialInventory.add(Product("Water", Money(100)), 5);
  initialInventory.add(Product("Coffee", Money(150)), 5);
  repository.save(initialInventory);

  // 3. Inject Dependencies into Application Core
  VendingMachineService service(repository, paymentGateway);

  // 4. Drive the Application via Driving Adapter
  // Pass service twice because it implements both interfaces
  // (IVendingMachineService, IMaintenanceService)
  ConsoleAdapter consoleAdapter(service, service);

  // 5. Run
  consoleAdapter.run();

  return 0;
}