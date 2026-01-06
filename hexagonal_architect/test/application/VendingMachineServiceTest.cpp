#include "application/VendingMachineService.hpp"
#include "domain/Product.hpp"
#include "ports/outbound/IInventoryRepository.hpp"
#include "ports/outbound/IPaymentGateway.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace vending_machine::ports::inbound;
using namespace vending_machine::ports::outbound;
using namespace vending_machine::domain;
using ::testing::_;
using ::testing::Return;

namespace vending_machine::application::test {

// Mock Inventory Repository
class MockInventoryRepository : public IInventoryRepository {
public:
  MOCK_METHOD(Inventory, getInventory, (), (override));
  MOCK_METHOD(void, save, (const Inventory &), (override));
};

// Mock Payment Gateway
class MockPaymentGateway : public IPaymentGateway {
public:
  MOCK_METHOD(bool, pay, (int amount), (override));
};

class VendingMachineServiceTest : public ::testing::Test {
protected:
  MockInventoryRepository mockRepo;
  MockPaymentGateway mockPayment;
  // Service will be initialized in tests
};

TEST_F(VendingMachineServiceTest, ShouldInitializeWithInventoryFromRepo) {
  Inventory initialInventory;
  initialInventory.add(Product("Cola", Money(100)), 5);

  EXPECT_CALL(mockRepo, getInventory()).WillOnce(Return(initialInventory));

  VendingMachineService service(mockRepo, mockPayment);

  // Check if inventory is loaded (indirectly via purchase capability)
  service.insertMoney(Money(100));
  EXPECT_NO_THROW(service.selectProduct("Cola"));
}

TEST_F(VendingMachineServiceTest, ShouldSaveInventoryAfterPurchase) {
  Inventory initialInventory;
  initialInventory.add(Product("Cola", Money(100)), 5);

  EXPECT_CALL(mockRepo, getInventory()).WillOnce(Return(initialInventory));

  EXPECT_CALL(mockRepo, save(_)).Times(1);

  VendingMachineService service(mockRepo, mockPayment);
  service.insertMoney(Money(100));
  service.selectProduct("Cola");
}

TEST_F(VendingMachineServiceTest, ShouldProcessElectronicMoneyPurchase) {
  Inventory initialInventory;
  initialInventory.add(Product("Tea", Money(120)), 5);

  EXPECT_CALL(mockRepo, getInventory()).WillOnce(Return(initialInventory));

  EXPECT_CALL(mockPayment, pay(120)).WillOnce(Return(true));

  EXPECT_CALL(mockRepo, save(_)).Times(1);

  VendingMachineService service(mockRepo, mockPayment);

  // Expect success (no exception), and correct product returned
  Product purchased = service.purchaseWithElectronicMoney("Tea");
  EXPECT_EQ(purchased.name(), "Tea");
}

TEST_F(VendingMachineServiceTest,
       ShouldFailElectronicMoneyPurchaseIfPaymentFails) {
  Inventory initialInventory;
  initialInventory.add(Product("Tea", Money(120)), 5);

  EXPECT_CALL(mockRepo, getInventory()).WillOnce(Return(initialInventory));

  EXPECT_CALL(mockPayment, pay(120)).WillOnce(Return(false));

  EXPECT_CALL(mockRepo, save(_)).Times(0);

  VendingMachineService service(mockRepo, mockPayment);

  // Expect exception
  EXPECT_THROW(
      { service.purchaseWithElectronicMoney("Tea"); }, std::runtime_error);
}

} // namespace vending_machine::application::test