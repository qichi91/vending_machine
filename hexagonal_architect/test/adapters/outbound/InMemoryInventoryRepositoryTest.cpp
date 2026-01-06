#include "adapters/outbound/memory_repository/InMemoryInventoryRepository.hpp"
#include "domain/Product.hpp"
#include <gtest/gtest.h>

namespace vending_machine::adapters::outbound::test {

using namespace domain;

class InMemoryInventoryRepositoryTest : public ::testing::Test {};

TEST_F(InMemoryInventoryRepositoryTest, ShouldSaveAndRetrieveInventory) {
  InMemoryInventoryRepository repo;

  Inventory inventory;
  Product cola("Cola", Money(100));
  inventory.add(cola, 5);

  repo.save(inventory);

  Inventory retrieved = repo.getInventory();
  EXPECT_EQ(retrieved.getCount(cola), 5);
  EXPECT_TRUE(retrieved.hasStock(cola));
}

TEST_F(InMemoryInventoryRepositoryTest, ShouldUpdateExistingInventory) {
  InMemoryInventoryRepository repo;
  Product cola("Cola", Money(100));

  Inventory inv1;
  inv1.add(cola, 5);
  repo.save(inv1);

  Inventory inv2 = repo.getInventory();
  inv2.reduce(cola); // 4
  repo.save(inv2);

  Inventory inv3 = repo.getInventory();
  EXPECT_EQ(inv3.getCount(cola), 4);
}

} // namespace vending_machine::adapters::outbound::test