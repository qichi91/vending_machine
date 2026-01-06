#include "domain/Money.hpp"
#include "domain/Product.hpp"
#include "domain/VendingMachine.hpp"
#include <gtest/gtest.h>

namespace vending_machine::domain::test {

class VendingMachineDomainTest : public ::testing::Test {
protected:
  void SetUp() override {}
};

TEST_F(VendingMachineDomainTest, ShouldRecordSalesWhenPurchasingWithCash) {
  VendingMachine vm;
  Product cola("Cola", Money(100));
  vm.addStock(cola, 5);
  vm.insertMoney(Money(100));

  vm.purchase("Cola");

  EXPECT_EQ(vm.getSales(), 100);
}

TEST_F(VendingMachineDomainTest, ShouldPurchaseWithElectronicMoney) {
  VendingMachine vm;
  Product cola("Cola", Money(100));
  vm.addStock(cola, 5);

  // No cash inserted
  EXPECT_EQ(vm.currentAmount(), 0);

  // Purchase with E-Money (External validation assumed)
  Product p = vm.purchaseViaElectronicMoney("Cola");

  EXPECT_EQ(p.name(), "Cola");
  EXPECT_EQ(vm.getSales(), 100);               // Sales should increase
  EXPECT_EQ(vm.currentAmount(), 0);            // Cash amount unchanged
  EXPECT_EQ(vm.inventory().getCount(cola), 4); // Stock reduced
}

TEST_F(VendingMachineDomainTest, ShouldFailEMoneyPurchaseIfOutOfStock) {
  VendingMachine vm;
  Product cola("Cola", Money(100));
  // No stock added

  EXPECT_THROW(vm.purchaseViaElectronicMoney("Cola"), std::runtime_error);
}

} // namespace vending_machine::domain::test