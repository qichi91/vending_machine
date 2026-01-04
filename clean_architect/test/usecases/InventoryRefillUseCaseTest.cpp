#include "usecases/usecases/InventoryRefillUseCase.hpp"
#include "domain/common/Price.hpp"
#include "domain/common/Quantity.hpp"
#include "domain/inventory/Inventory.hpp"
#include "domain/inventory/ProductInfo.hpp"
#include "domain/inventory/ProductName.hpp"
#include "domain/inventory/ProductSlot.hpp"
#include "domain/inventory/SlotId.hpp"
#include <gtest/gtest.h>

namespace vending_machine {
namespace usecases {

class InventoryRefillUseCaseTest : public ::testing::Test {
protected:
  void SetUp() override {
    // スロット1: Coke (120円)
    domain::ProductSlot slot1(
        domain::SlotId(1),
        domain::ProductInfo(domain::ProductName("Coke"), domain::Price(120)),
        domain::Quantity(0));
    inventory.addSlot(slot1);

    // スロット2: Pepsi (150円)
    domain::ProductSlot slot2(
        domain::SlotId(2),
        domain::ProductInfo(domain::ProductName("Pepsi"), domain::Price(150)),
        domain::Quantity(0));
    inventory.addSlot(slot2);

    // スロット3: Water (100円)
    domain::ProductSlot slot3(
        domain::SlotId(3),
        domain::ProductInfo(domain::ProductName("Water"), domain::Price(100)),
        domain::Quantity(0));
    inventory.addSlot(slot3);

    use_case = std::make_unique<InventoryRefillUseCase>(inventory);
  }

  domain::Inventory inventory;
  std::unique_ptr<InventoryRefillUseCase> use_case;
};

// テスト1: 単一スロットの補充
TEST_F(InventoryRefillUseCaseTest, CanRefillSingleSlot) {
  use_case->refillSlot(domain::SlotId(1), domain::Quantity(10));
  EXPECT_EQ(10, use_case->getSlotQuantity(domain::SlotId(1)).getValue());
}

// テスト2: 複数スロットの補充
TEST_F(InventoryRefillUseCaseTest, CanRefillMultipleSlots) {
  std::vector<std::pair<domain::SlotId, domain::Quantity>> refills = {
      {domain::SlotId(1), domain::Quantity(10)},
      {domain::SlotId(2), domain::Quantity(8)},
      {domain::SlotId(3), domain::Quantity(15)}};

  use_case->refillMultiple(refills);

  EXPECT_EQ(10, use_case->getSlotQuantity(domain::SlotId(1)).getValue());
  EXPECT_EQ(8, use_case->getSlotQuantity(domain::SlotId(2)).getValue());
  EXPECT_EQ(15, use_case->getSlotQuantity(domain::SlotId(3)).getValue());
}

// テスト3: 複数回の補充（累積）
TEST_F(InventoryRefillUseCaseTest, CanRefillMultipleTimes) {
  use_case->refillSlot(domain::SlotId(1), domain::Quantity(5));
  EXPECT_EQ(5, use_case->getSlotQuantity(domain::SlotId(1)).getValue());

  use_case->refillSlot(domain::SlotId(1), domain::Quantity(3));
  EXPECT_EQ(8, use_case->getSlotQuantity(domain::SlotId(1)).getValue());
}

// テスト4: 在庫確認
TEST_F(InventoryRefillUseCaseTest, CanCheckSlotQuantity) {
  use_case->refillSlot(domain::SlotId(2), domain::Quantity(12));
  domain::Quantity quantity = use_case->getSlotQuantity(domain::SlotId(2));
  EXPECT_EQ(12, quantity.getValue());
}

// テスト5: 複数スロットの補充後に追加補充
TEST_F(InventoryRefillUseCaseTest, CanRefillAfterInitialRefill) {
  std::vector<std::pair<domain::SlotId, domain::Quantity>> initial_refills = {
      {domain::SlotId(1), domain::Quantity(10)},
      {domain::SlotId(2), domain::Quantity(8)}};
  use_case->refillMultiple(initial_refills);

  use_case->refillSlot(domain::SlotId(1), domain::Quantity(5));

  EXPECT_EQ(15, use_case->getSlotQuantity(domain::SlotId(1)).getValue());
  EXPECT_EQ(8, use_case->getSlotQuantity(domain::SlotId(2)).getValue());
}

// テスト6: すべてのスロットを補充
TEST_F(InventoryRefillUseCaseTest, CanRefillAllSlots) {
  std::vector<std::pair<domain::SlotId, domain::Quantity>> all_refills = {
      {domain::SlotId(1), domain::Quantity(10)},
      {domain::SlotId(2), domain::Quantity(15)},
      {domain::SlotId(3), domain::Quantity(20)}};

  use_case->refillMultiple(all_refills);

  EXPECT_EQ(10, use_case->getSlotQuantity(domain::SlotId(1)).getValue());
  EXPECT_EQ(15, use_case->getSlotQuantity(domain::SlotId(2)).getValue());
  EXPECT_EQ(20, use_case->getSlotQuantity(domain::SlotId(3)).getValue());
}

} // namespace usecases
} // namespace vending_machine
