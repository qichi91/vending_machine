#include "domain/VendingMachine.hpp"
#include "domain/Money.hpp"
#include "domain/Product.hpp"
#include <gtest/gtest.h>

namespace vending_machine::domain::test {

class VendingMachineTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Setup with empty inventory
  }
};

TEST_F(VendingMachineTest, ShouldAcceptMoney) {
  VendingMachine vm;
  vm.insertMoney(Money(100));
  EXPECT_EQ(vm.currentAmount(), 100);

  vm.insertMoney(Money(50));
  EXPECT_EQ(vm.currentAmount(), 150);
}

TEST_F(VendingMachineTest, ShouldPurchaseProductAssumingStockExists) {
  // VendingMachine creates its own internal inventory or we pass it?
  // Let's say we pass inventory or add stock to vm.
  VendingMachine vm;
  Product cola("Cola", Money(100));
  vm.addStock(cola, 5);

  vm.insertMoney(Money(100));
  Product purchased = vm.purchase("Cola");

  EXPECT_EQ(purchased.name(), "Cola");
  EXPECT_EQ(vm.currentAmount(), 0); // Price deducted
}

TEST_F(VendingMachineTest, ShouldThrowExceptionIfInsufficientFunds) {
  VendingMachine vm;
  Product cola("Cola", Money(100));
  vm.addStock(cola, 5);

  vm.insertMoney(Money(50));
  EXPECT_THROW(vm.purchase("Cola"), std::runtime_error);
}

TEST_F(VendingMachineTest, ShouldRefundRemainingAmount) {
  VendingMachine vm;
  vm.insertMoney(Money(100));
  vm.insertMoney(Money(50));

  // Refund logic needs to return list of Money logic potentially
  // For now, let's assume it clears amount
  int refundedAmount = vm.refund();
  EXPECT_EQ(refundedAmount, 150);
  EXPECT_EQ(vm.currentAmount(), 0);
}

} // namespace vending_machine::domain::test