/**
 * @file SlotIdTest.cpp
 * @brief SlotId Value Object のユニットテスト
 *
 * テスト方針:
 * - 同値分割法: 正の値、0、負の値（エラー）
 * - 境界値分析: 0（無効）、1（最小）、最大スロット数
 */

#include "domain/inventory/SlotId.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

namespace vending_machine {
namespace domain {
namespace test {

class SlotIdTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

// ========================================
// 正常系テスト: 生成と取得
// ========================================

/**
 * @test 正の値でSlotIdオブジェクトを生成できる
 */
TEST_F(SlotIdTest, CanCreateWithPositiveValue) {
  SlotId slot_id(1);
  EXPECT_EQ(1, slot_id.getValue());
}

/**
 * @test 1でSlotIdオブジェクトを生成できる（最小値）
 */
TEST_F(SlotIdTest, CanCreateWithOne) {
  SlotId slot_id(1);
  EXPECT_EQ(1, slot_id.getValue());
}

/**
 * @test 大きな値でSlotIdオブジェクトを生成できる
 */
TEST_F(SlotIdTest, CanCreateWithLargeValue) {
  SlotId slot_id(100);
  EXPECT_EQ(100, slot_id.getValue());
}

// ========================================
// 異常系テスト: 不正な値
// ========================================

/**
 * @test 0でSlotIdオブジェクトを生成しようとすると例外が発生する（境界値）
 */
TEST_F(SlotIdTest, ThrowsExceptionWhenCreatingWithZero) {
  EXPECT_THROW({ SlotId slot_id(0); }, std::invalid_argument);
}

/**
 * @test 負の値でSlotIdオブジェクトを生成しようとすると例外が発生する
 */
TEST_F(SlotIdTest, ThrowsExceptionWhenCreatingWithNegativeValue) {
  EXPECT_THROW({ SlotId slot_id(-1); }, std::invalid_argument);
}

/**
 * @test 負の大きな値でSlotIdオブジェクトを生成しようとすると例外が発生する
 */
TEST_F(SlotIdTest, ThrowsExceptionWhenCreatingWithLargeNegativeValue) {
  EXPECT_THROW({ SlotId slot_id(-100); }, std::invalid_argument);
}

// ========================================
// 比較演算子テスト
// ========================================

/**
 * @test 等価演算子が正しく動作する
 */
TEST_F(SlotIdTest, EqualityOperatorWorks) {
  SlotId slot_id1(1);
  SlotId slot_id2(1);
  SlotId slot_id3(2);

  EXPECT_TRUE(slot_id1 == slot_id2);
  EXPECT_FALSE(slot_id1 == slot_id3);
}

/**
 * @test 不等価演算子が正しく動作する
 */
TEST_F(SlotIdTest, InequalityOperatorWorks) {
  SlotId slot_id1(1);
  SlotId slot_id2(2);
  SlotId slot_id3(1);

  EXPECT_TRUE(slot_id1 != slot_id2);
  EXPECT_FALSE(slot_id1 != slot_id3);
}

/**
 * @test 小なり演算子が正しく動作する
 */
TEST_F(SlotIdTest, LessThanOperatorWorks) {
  SlotId slot_id1(1);
  SlotId slot_id2(2);
  SlotId slot_id3(2);

  EXPECT_TRUE(slot_id1 < slot_id2);
  EXPECT_FALSE(slot_id2 < slot_id1);
  EXPECT_FALSE(slot_id2 < slot_id3);
}

/**
 * @test 大なり演算子が正しく動作する
 */
TEST_F(SlotIdTest, GreaterThanOperatorWorks) {
  SlotId slot_id1(2);
  SlotId slot_id2(1);
  SlotId slot_id3(2);

  EXPECT_TRUE(slot_id1 > slot_id2);
  EXPECT_FALSE(slot_id2 > slot_id1);
  EXPECT_FALSE(slot_id1 > slot_id3);
}

} // namespace test
} // namespace domain
} // namespace vending_machine
