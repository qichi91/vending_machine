/**
 * @file InventoryTest.cpp
 * @brief Inventory Aggregate のユニットテスト
 *
 * テスト方針:
 * - スロット管理: 追加、取得、検索
 * - 在庫操作: 販売、補充
 * - エラーハンドリング: 無効なスロット、重複登録
 */

#include "src/domain/inventory/Inventory.hpp"
#include "src/domain/common/Price.hpp"
#include "src/domain/common/Quantity.hpp"
#include "src/domain/inventory/ProductInfo.hpp"
#include "src/domain/inventory/ProductName.hpp"
#include "src/domain/inventory/ProductSlot.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

namespace vending_machine {
namespace domain {
namespace test {

class InventoryTest : public ::testing::Test {
protected:
  ProductName cola_name{ProductName("Cola")};
  Price cola_price{100};
  ProductInfo cola{cola_name, cola_price};

  ProductName coffee_name{ProductName("Coffee")};
  Price coffee_price{120};
  ProductInfo coffee{coffee_name, coffee_price};

  ProductName juice_name{ProductName("Juice")};
  Price juice_price{150};
  ProductInfo juice{juice_name, juice_price};

  void SetUp() override {}
  void TearDown() override {}
};

// ========================================
// 正常系テスト: 生成と初期状態
// ========================================

/**
 * @test 空のInventoryを生成できる
 */
TEST_F(InventoryTest, CanCreateEmptyInventory) {
  Inventory inventory;
  // 空の状況であることをテスト
  EXPECT_THROW({ inventory.getSlot(SlotId(1)); }, std::invalid_argument);
}

// ========================================
// スロット追加テスト
// ========================================

/**
 * @test スロットを追加できる
 */
TEST_F(InventoryTest, CanAddSlot) {
  Inventory inventory;
  ProductSlot slot(SlotId(1), cola, Quantity(10));

  inventory.addSlot(slot);

  // 追加後、取得できることを確認
  const ProductSlot &retrieved = inventory.getSlot(SlotId(1));
  EXPECT_EQ(1, retrieved.getSlotId().getValue());
  EXPECT_EQ("Cola", retrieved.getProductInfo().getName().getValue());
}

/**
 * @test 複数のスロットを追加できる
 */
TEST_F(InventoryTest, CanAddMultipleSlots) {
  Inventory inventory;

  inventory.addSlot(ProductSlot(SlotId(1), cola, Quantity(10)));
  inventory.addSlot(ProductSlot(SlotId(2), coffee, Quantity(20)));
  inventory.addSlot(ProductSlot(SlotId(3), juice, Quantity(15)));

  EXPECT_EQ(1, inventory.getSlot(SlotId(1)).getSlotId().getValue());
  EXPECT_EQ(2, inventory.getSlot(SlotId(2)).getSlotId().getValue());
  EXPECT_EQ(3, inventory.getSlot(SlotId(3)).getSlotId().getValue());
}

/**
 * @test 売り切れの状態のスロットを追加できる
 */
TEST_F(InventoryTest, CanAddEmptySlot) {
  Inventory inventory;

  inventory.addSlot(ProductSlot(SlotId(1), cola, Quantity(0)));

  const ProductSlot &slot = inventory.getSlot(SlotId(1));
  EXPECT_FALSE(slot.isAvailable());
}

/**
 * @test 同じスロットIDを2回追加しようとすると例外が発生する
 */
TEST_F(InventoryTest, ThrowsExceptionWhenAddingDuplicateSlotId) {
  Inventory inventory;

  inventory.addSlot(ProductSlot(SlotId(1), cola, Quantity(10)));

  EXPECT_THROW(
      { inventory.addSlot(ProductSlot(SlotId(1), coffee, Quantity(20))); },
      std::invalid_argument);
}

// ========================================
// スロット取得テスト
// ========================================

/**
 * @test 存在するスロットIDでスロットを取得できる
 */
TEST_F(InventoryTest, CanRetrieveExistingSlot) {
  Inventory inventory;
  inventory.addSlot(ProductSlot(SlotId(5), cola, Quantity(10)));

  const ProductSlot &slot = inventory.getSlot(SlotId(5));
  EXPECT_EQ(5, slot.getSlotId().getValue());
}

/**
 * @test 存在しないスロットIDでアクセスしようとすると例外が発生する
 */
TEST_F(InventoryTest, ThrowsExceptionWhenAccessingNonExistentSlot) {
  Inventory inventory;
  inventory.addSlot(ProductSlot(SlotId(1), cola, Quantity(10)));

  EXPECT_THROW({ inventory.getSlot(SlotId(2)); }, std::invalid_argument);
}

/**
 * @test 複数スロット中の特定スロットを正確に取得できる
 */
TEST_F(InventoryTest, CanRetrieveCorrectSlotAmongMultiple) {
  Inventory inventory;

  inventory.addSlot(ProductSlot(SlotId(1), cola, Quantity(10)));
  inventory.addSlot(ProductSlot(SlotId(2), coffee, Quantity(20)));
  inventory.addSlot(ProductSlot(SlotId(3), juice, Quantity(30)));

  const ProductSlot &slot2 = inventory.getSlot(SlotId(2));
  EXPECT_EQ("Coffee", slot2.getProductInfo().getName().getValue());
  EXPECT_EQ(120, slot2.getProductInfo().getPrice().getRawValue());
  EXPECT_EQ(20, slot2.getStock().getValue());
}

// ========================================
// 在庫販売テスト
// ========================================

/**
 * @test スロットから商品を販売できる
 */
TEST_F(InventoryTest, CanDispenseFromSlot) {
  Inventory inventory;
  inventory.addSlot(ProductSlot(SlotId(1), cola, Quantity(10)));

  inventory.dispense(SlotId(1));

  EXPECT_EQ(9, inventory.getSlot(SlotId(1)).getStock().getValue());
}

/**
 * @test 最後の1個を販売できる（境界値）
 */
TEST_F(InventoryTest, CanDispenseLastOne) {
  Inventory inventory;
  inventory.addSlot(ProductSlot(SlotId(1), cola, Quantity(1)));

  inventory.dispense(SlotId(1));

  EXPECT_EQ(0, inventory.getSlot(SlotId(1)).getStock().getValue());
  EXPECT_FALSE(inventory.getSlot(SlotId(1)).isAvailable());
}

/**
 * @test 売り切れの状態から販売しようとすると例外が発生する
 */
TEST_F(InventoryTest, ThrowsExceptionWhenDispensingFromEmpty) {
  Inventory inventory;
  inventory.addSlot(ProductSlot(SlotId(1), cola, Quantity(0)));

  EXPECT_THROW({ inventory.dispense(SlotId(1)); }, std::domain_error);
}

/**
 * @test 存在しないスロットから販売しようとすると例外が発生する
 */
TEST_F(InventoryTest, ThrowsExceptionWhenDispensingFromNonExistentSlot) {
  Inventory inventory;
  inventory.addSlot(ProductSlot(SlotId(1), cola, Quantity(10)));

  EXPECT_THROW({ inventory.dispense(SlotId(2)); }, std::invalid_argument);
}

/**
 * @test 複数回の販売により在庫が正しく減少する
 */
TEST_F(InventoryTest, CanDispenseMultipleTimes) {
  Inventory inventory;
  inventory.addSlot(ProductSlot(SlotId(1), cola, Quantity(10)));

  inventory.dispense(SlotId(1));
  inventory.dispense(SlotId(1));
  inventory.dispense(SlotId(1));

  EXPECT_EQ(7, inventory.getSlot(SlotId(1)).getStock().getValue());
}

// ========================================
// 在庫補充テスト
// ========================================

/**
 * @test スロットに在庫を補充できる
 */
TEST_F(InventoryTest, CanRefillSlot) {
  Inventory inventory;
  inventory.addSlot(ProductSlot(SlotId(1), cola, Quantity(10)));

  inventory.refill(SlotId(1), Quantity(5));

  EXPECT_EQ(15, inventory.getSlot(SlotId(1)).getStock().getValue());
}

/**
 * @test 売り切れのスロットに補充できる
 */
TEST_F(InventoryTest, CanRefillEmptySlot) {
  Inventory inventory;
  inventory.addSlot(ProductSlot(SlotId(1), cola, Quantity(0)));

  inventory.refill(SlotId(1), Quantity(20));

  EXPECT_EQ(20, inventory.getSlot(SlotId(1)).getStock().getValue());
  EXPECT_TRUE(inventory.getSlot(SlotId(1)).isAvailable());
}

/**
 * @test 最大収容数まで補充できる（境界値）
 */
TEST_F(InventoryTest, CanRefillToMaxCapacity) {
  Inventory inventory;
  inventory.addSlot(ProductSlot(SlotId(1), cola, Quantity(45)));

  inventory.refill(SlotId(1), Quantity(5));

  EXPECT_EQ(50, inventory.getSlot(SlotId(1)).getStock().getValue());
}

/**
 * @test 最大収容数を超えて補充しようとすると例外が発生する
 */
TEST_F(InventoryTest, ThrowsExceptionWhenRefillExceedsCapacity) {
  Inventory inventory;
  inventory.addSlot(ProductSlot(SlotId(1), cola, Quantity(45)));

  EXPECT_THROW(
      { inventory.refill(SlotId(1), Quantity(10)); }, std::domain_error);
}

/**
 * @test 存在しないスロットに補充しようとすると例外が発生する
 */
TEST_F(InventoryTest, ThrowsExceptionWhenRefillNonExistentSlot) {
  Inventory inventory;
  inventory.addSlot(ProductSlot(SlotId(1), cola, Quantity(10)));

  EXPECT_THROW(
      { inventory.refill(SlotId(2), Quantity(5)); }, std::invalid_argument);
}

// ========================================
// 複合操作テスト
// ========================================

/**
 * @test 販売と補充を組み合わせた操作ができる
 */
TEST_F(InventoryTest, CanPerformCombinedOperations) {
  Inventory inventory;
  inventory.addSlot(ProductSlot(SlotId(1), cola, Quantity(10)));

  inventory.dispense(SlotId(1));
  inventory.dispense(SlotId(1));

  EXPECT_EQ(8, inventory.getSlot(SlotId(1)).getStock().getValue());

  inventory.refill(SlotId(1), Quantity(12));

  EXPECT_EQ(20, inventory.getSlot(SlotId(1)).getStock().getValue());
}

/**
 * @test 複数スロット間での独立した在庫操作
 */
TEST_F(InventoryTest, OperationsOnDifferentSlotsAreIndependent) {
  Inventory inventory;
  inventory.addSlot(ProductSlot(SlotId(1), cola, Quantity(10)));
  inventory.addSlot(ProductSlot(SlotId(2), coffee, Quantity(20)));

  inventory.dispense(SlotId(1));
  inventory.dispense(SlotId(1));

  inventory.refill(SlotId(2), Quantity(10));

  EXPECT_EQ(8, inventory.getSlot(SlotId(1)).getStock().getValue());
  EXPECT_EQ(30, inventory.getSlot(SlotId(2)).getStock().getValue());
}

} // namespace test
} // namespace domain
} // namespace vending_machine
