/**
 * @file ProductSlotTest.cpp
 * @brief ProductSlot Entity のユニットテスト
 *
 * テスト方針:
 * - 在庫の増減
 * - 在庫状態の判定
 * - スロット情報の取得
 */

#include "src/domain/inventory/ProductSlot.hpp"
#include "src/domain/common/Price.hpp"
#include "src/domain/common/Quantity.hpp"
#include "src/domain/inventory/ProductInfo.hpp"
#include "src/domain/inventory/ProductName.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

namespace vending_machine {
namespace domain {
namespace test {

class ProductSlotTest : public ::testing::Test {
protected:
  ProductName cola_name{ProductName("Cola")};
  Price cola_price{100};
  ProductInfo cola{cola_name, cola_price};

  void SetUp() override {}
  void TearDown() override {}
};

// ========================================
// 正常系テスト: 生成と取得
// ========================================

/**
 * @test スロットIDと商品情報と在庫数でProductSlotを生成できる
 */
TEST_F(ProductSlotTest, CanCreateProductSlot) {
  ProductSlot slot(SlotId(1), cola, Quantity(10));

  EXPECT_EQ(1, slot.getSlotId().getValue());
  EXPECT_EQ("Cola", slot.getProductInfo().getName().getValue());
  EXPECT_EQ(100, slot.getProductInfo().getPrice().getRawValue());
  EXPECT_EQ(10, slot.getStock().getValue());
}

/**
 * @test 最大収容数で生成できる
 */
TEST_F(ProductSlotTest, CanCreateWithMaxCapacity) {
  ProductSlot slot(SlotId(1), cola, Quantity(50));

  EXPECT_EQ(50, slot.getStock().getValue());
}

/**
 * @test 0（売り切れ）の状態で生成できる
 */
TEST_F(ProductSlotTest, CanCreateWithZeroStock) {
  ProductSlot slot(SlotId(1), cola, Quantity(0));

  EXPECT_EQ(0, slot.getStock().getValue());
}

// ========================================
// 在庫状態の判定テスト
// ========================================

/**
 * @test 在庫があるかどうかを判定できる
 */
TEST_F(ProductSlotTest, CanCheckIfAvailable) {
  ProductSlot in_stock(SlotId(1), cola, Quantity(10));
  ProductSlot out_of_stock(SlotId(2), cola, Quantity(0));

  EXPECT_TRUE(in_stock.isAvailable());
  EXPECT_FALSE(out_of_stock.isAvailable());
}

/**
 * @test 1個の在庫がある場合はavailable（境界値）
 */
TEST_F(ProductSlotTest, IsAvailableWithOneStock) {
  ProductSlot slot(SlotId(1), cola, Quantity(1));

  EXPECT_TRUE(slot.isAvailable());
}

// ========================================
// 在庫減少テスト（販売）
// ========================================

/**
 * @test 在庫を1個減らせる
 */
TEST_F(ProductSlotTest, CanDispenseOne) {
  ProductSlot slot(SlotId(1), cola, Quantity(10));

  slot.dispense();

  EXPECT_EQ(9, slot.getStock().getValue());
}

/**
 * @test 在庫を複数回減らせる
 */
TEST_F(ProductSlotTest, CanDispenseMultipleTimes) {
  ProductSlot slot(SlotId(1), cola, Quantity(10));

  slot.dispense();
  slot.dispense();
  slot.dispense();

  EXPECT_EQ(7, slot.getStock().getValue());
}

/**
 * @test 最後の1個を減らせる（境界値）
 */
TEST_F(ProductSlotTest, CanDispenseLastOne) {
  ProductSlot slot(SlotId(1), cola, Quantity(1));

  slot.dispense();

  EXPECT_EQ(0, slot.getStock().getValue());
  EXPECT_FALSE(slot.isAvailable());
}

/**
 * @test 0個の状態から減らそうとすると例外が発生する
 */
TEST_F(ProductSlotTest, ThrowsExceptionWhenDispensingFromEmpty) {
  ProductSlot slot(SlotId(1), cola, Quantity(0));

  EXPECT_THROW({ slot.dispense(); }, std::domain_error);
}

// ========================================
// 在庫増加テスト（補充）
// ========================================

/**
 * @test 在庫を補充できる
 */
TEST_F(ProductSlotTest, CanRefill) {
  ProductSlot slot(SlotId(1), cola, Quantity(10));

  slot.refill(Quantity(5));

  EXPECT_EQ(15, slot.getStock().getValue());
}

/**
 * @test 売り切れの状態から補充できる
 */
TEST_F(ProductSlotTest, CanRefillFromEmpty) {
  ProductSlot slot(SlotId(1), cola, Quantity(0));

  slot.refill(Quantity(20));

  EXPECT_EQ(20, slot.getStock().getValue());
  EXPECT_TRUE(slot.isAvailable());
}

/**
 * @test 最大収容数まで補充できる（境界値）
 */
TEST_F(ProductSlotTest, CanRefillToMaxCapacity) {
  ProductSlot slot(SlotId(1), cola, Quantity(45));

  slot.refill(Quantity(5));

  EXPECT_EQ(50, slot.getStock().getValue());
}

/**
 * @test 最大収容数を超えて補充しようとすると例外が発生する
 */
TEST_F(ProductSlotTest, ThrowsExceptionWhenRefillExceedsCapacity) {
  ProductSlot slot(SlotId(1), cola, Quantity(45));

  EXPECT_THROW({ slot.refill(Quantity(10)); }, std::domain_error);
}

/**
 * @test 0個の補充は何も変わらない
 */
TEST_F(ProductSlotTest, RefillByZeroDoesNotChange) {
  ProductSlot slot(SlotId(1), cola, Quantity(10));

  slot.refill(Quantity(0));

  EXPECT_EQ(10, slot.getStock().getValue());
}

// ========================================
// 商品情報の一致性テスト
// ========================================

/**
 * @test 商品情報は変更されない（不変性）
 */
TEST_F(ProductSlotTest, ProductInfoIsImmutable) {
  ProductSlot slot(SlotId(1), cola, Quantity(10));

  // 在庫を操作しても
  slot.dispense();
  slot.refill(Quantity(5));

  // 商品情報は変わらない
  EXPECT_EQ("Cola", slot.getProductInfo().getName().getValue());
  EXPECT_EQ(100, slot.getProductInfo().getPrice().getRawValue());
}

/**
 * @test スロットIDは変更されない（不変性）
 */
TEST_F(ProductSlotTest, SlotIdIsImmutable) {
  ProductSlot slot(SlotId(5), cola, Quantity(10));

  slot.dispense();

  EXPECT_EQ(5, slot.getSlotId().getValue());
}

} // namespace test
} // namespace domain
} // namespace vending_machine
