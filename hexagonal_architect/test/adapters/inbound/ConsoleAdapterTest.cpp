#include "adapters/inbound/console/ConsoleAdapter.hpp"
#include "ports/inbound/IMaintenanceService.hpp"
#include "ports/inbound/IVendingMachineService.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sstream>

using namespace vending_machine::ports::inbound;
using namespace vending_machine::adapters::inbound;
using ::testing::_;

namespace vending_machine::adapters::inbound::test {

class MockVendingMachineService : public IVendingMachineService {
public:
  MOCK_METHOD(void, insertMoney, (const domain::Money &), (override));
  MOCK_METHOD(domain::Product, selectProduct, (const std::string &),
              (override));
  MOCK_METHOD(int, getCurrentAmount, (), (const, override));
  MOCK_METHOD(std::vector<domain::Money>, getRefund, (), (override));
  MOCK_METHOD(domain::Product, purchaseWithElectronicMoney,
              (const std::string &), (override));
};

class MockMaintenanceService : public IMaintenanceService {
public:
  MOCK_METHOD(void, addStock, (const std::string &, int, int), (override));
  MOCK_METHOD(int, getTotalSales, (), (const, override));
};

class ConsoleAdapterTest : public ::testing::Test {
protected:
  MockVendingMachineService mockService;
  MockMaintenanceService mockMaintenance;
};

TEST_F(ConsoleAdapterTest, ShouldParseInsertCommand) {
  std::stringstream input("insert 100\nexit\n");
  std::stringstream output;

  ConsoleAdapter adapter(mockService, mockMaintenance, input, output);

  EXPECT_CALL(mockService, insertMoney(domain::Money(100))).Times(1);

  adapter.run();
}

TEST_F(ConsoleAdapterTest, ShouldParseSelectCommand) {
  std::stringstream input("select Cola\nexit\n");
  std::stringstream output;

  ConsoleAdapter adapter(mockService, mockMaintenance, input, output);

  EXPECT_CALL(mockService, selectProduct("Cola"))
      .WillOnce(::testing::Return(domain::Product("Cola", domain::Money(100))));

  adapter.run();
}

TEST_F(ConsoleAdapterTest, ShouldParseEMoneyCommand) {
  std::stringstream input("emoney Cola\nexit\n");
  std::stringstream output;

  ConsoleAdapter adapter(mockService, mockMaintenance, input, output);

  EXPECT_CALL(mockService, purchaseWithElectronicMoney("Cola"))
      .WillOnce(::testing::Return(domain::Product("Cola", domain::Money(100))));

  adapter.run();
}

TEST_F(ConsoleAdapterTest, ShouldParseRestockCommand) {
  std::stringstream input("restock Cola 100 5\nexit\n");
  std::stringstream output;

  ConsoleAdapter adapter(mockService, mockMaintenance, input, output);

  EXPECT_CALL(mockMaintenance, addStock("Cola", 100, 5)).Times(1);

  adapter.run();
}

} // namespace vending_machine::adapters::inbound::test