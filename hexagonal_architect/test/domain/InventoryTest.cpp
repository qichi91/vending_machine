#include <gtest/gtest.h>
#include "domain/Inventory.hpp"
#include "domain/Product.hpp"
#include "domain/Money.hpp"

namespace vending_machine::domain::test {

class InventoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Common setup
    }
};

TEST_F(InventoryTest, ShouldAddAndReduceStock) {
    Inventory inventory;
    Product cola("Cola", Money(100));

    inventory.add(cola, 5);
    EXPECT_TRUE(inventory.hasStock(cola));
    EXPECT_EQ(inventory.getCount(cola), 5);

    inventory.reduce(cola);
    EXPECT_EQ(inventory.getCount(cola), 4);
}

TEST_F(InventoryTest, ShouldThrowExceptionWhenReducingEmptyStock) {
    Inventory inventory;
    Product cola("Cola", Money(100));

    // 在庫がないのに減らそうとする
    EXPECT_THROW(inventory.reduce(cola), std::runtime_error);
}

TEST_F(InventoryTest, ShouldManageMultipleProducts) {
    Inventory inventory;
    Product cola("Cola", Money(100));
    Product water("Water", Money(100)); // Same price, diff name

    inventory.add(cola, 1);
    inventory.add(water, 2);

    EXPECT_EQ(inventory.getCount(cola), 1);
    EXPECT_EQ(inventory.getCount(water), 2);
}

} // namespace vending_machine::domain::test