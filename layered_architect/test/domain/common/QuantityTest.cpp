/**
 * @file QuantityTest.cpp
 * @brief Quantity Value Object のユニットテスト
 *
 * テスト方針:
 * - 同値分割法: 0、正の値、負の値（エラー）、最大収容数
 * - 境界値分析: 0（売り切れ）、1、最大収容数、最大収容数+1（エラー）
 * - 操作テスト: 増加、減少
 */

#include "domain/common/Quantity.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

namespace vending_machine {
namespace domain {
namespace test {

/**
 * @brief Quantity Value Object の基本機能テスト
 */
class QuantityTest : public ::testing::Test {
protected:
  static constexpr int MAX_CAPACITY = 50; // 自販機の標準的な最大収容数

  void SetUp() override {
    // テストごとの初期化
  }

  void TearDown() override {
    // テストごとのクリーンアップ
  }
};

// ========================================
// 正常系テスト: 生成と取得
// ========================================

/**
 * @test 正の値でQuantityオブジェクトを生成できる
 */
TEST_F(QuantityTest, CanCreateWithPositiveValue) {
  Quantity quantity(10);
  EXPECT_EQ(10, quantity.getValue());
}

/**
 * @test 0（売り切れ）でQuantityオブジェクトを生成できる（境界値）
 */
TEST_F(QuantityTest, CanCreateWithZero) {
  Quantity quantity(0);
  EXPECT_EQ(0, quantity.getValue());
  EXPECT_TRUE(quantity.isZero());
}

/**
 * @test 1でQuantityオブジェクトを生成できる（境界値）
 */
TEST_F(QuantityTest, CanCreateWithOne) {
  Quantity quantity(1);
  EXPECT_EQ(1, quantity.getValue());
}

/**
 * @test 最大収容数でQuantityオブジェクトを生成できる（境界値）
 */
TEST_F(QuantityTest, CanCreateWithMaxCapacity) {
  Quantity quantity(MAX_CAPACITY);
  EXPECT_EQ(MAX_CAPACITY, quantity.getValue());
}

// ========================================
// 異常系テスト: 不正な値
// ========================================

/**
 * @test 負の値でQuantityオブジェクトを生成しようとすると例外が発生する
 */
TEST_F(QuantityTest, ThrowsExceptionWhenCreatingWithNegativeValue) {
  EXPECT_THROW({ Quantity quantity(-1); }, std::invalid_argument);
}

/**
 * @test 最大収容数を超える値で生成しようとすると例外が発生する（境界値）
 */
TEST_F(QuantityTest, ThrowsExceptionWhenCreatingWithValueExceedingMaxCapacity) {
  EXPECT_THROW({ Quantity quantity(MAX_CAPACITY + 1); }, std::invalid_argument);
}

/**
 * @test 大きな負の値で生成しようとすると例外が発生する
 */
TEST_F(QuantityTest, ThrowsExceptionWhenCreatingWithLargeNegativeValue) {
  EXPECT_THROW({ Quantity quantity(-100); }, std::invalid_argument);
}

// ========================================
// 状態判定テスト
// ========================================

/**
 * @test 0個の時にisZeroがtrueを返す
 */
TEST_F(QuantityTest, IsZeroReturnsTrueForZero) {
  Quantity quantity(0);
  EXPECT_TRUE(quantity.isZero());
}

/**
 * @test 0個でない時にisZeroがfalseを返す
 */
TEST_F(QuantityTest, IsZeroReturnsFalseForNonZero) {
  Quantity quantity(1);
  EXPECT_FALSE(quantity.isZero());
}

// ========================================
// 増加操作テスト
// ========================================

/**
 * @test 在庫を増加できる
 */
TEST_F(QuantityTest, CanIncrease) {
  Quantity quantity(10);
  Quantity increased = quantity.increase(5);

  EXPECT_EQ(15, increased.getValue());
  EXPECT_EQ(10, quantity.getValue()); // 元のオブジェクトは不変
}

/**
 * @test 0から増加できる（境界値）
 */
TEST_F(QuantityTest, CanIncreaseFromZero) {
  Quantity quantity(0);
  Quantity increased = quantity.increase(10);

  EXPECT_EQ(10, increased.getValue());
}

/**
 * @test 1増加できる（境界値）
 */
TEST_F(QuantityTest, CanIncreaseByOne) {
  Quantity quantity(10);
  Quantity increased = quantity.increase(1);

  EXPECT_EQ(11, increased.getValue());
}

/**
 * @test 最大収容数まで増加できる（境界値）
 */
TEST_F(QuantityTest, CanIncreaseToMaxCapacity) {
  Quantity quantity(MAX_CAPACITY - 5);
  Quantity increased = quantity.increase(5);

  EXPECT_EQ(MAX_CAPACITY, increased.getValue());
}

/**
 * @test 増加後も最大収容数を超えない範囲でないと例外が発生する
 */
TEST_F(QuantityTest, ThrowsExceptionWhenIncreaseExceedsMaxCapacity) {
  Quantity quantity(MAX_CAPACITY - 5);

  EXPECT_THROW({ quantity.increase(10); }, std::domain_error);
}

/**
 * @test 0を増加しても変わらない（境界値）
 */
TEST_F(QuantityTest, IncreaseByZeroDoesNotChange) {
  Quantity quantity(10);
  Quantity result = quantity.increase(0);

  EXPECT_EQ(10, result.getValue());
}

/**
 * @test 負の値で増加しようとすると例外が発生する
 */
TEST_F(QuantityTest, ThrowsExceptionWhenIncreasingByNegativeAmount) {
  Quantity quantity(10);

  EXPECT_THROW({ quantity.increase(-5); }, std::invalid_argument);
}

// ========================================
// 減少操作テスト
// ========================================

/**
 * @test 在庫を減少できる
 */
TEST_F(QuantityTest, CanDecrease) {
  Quantity quantity(10);
  Quantity decreased = quantity.decrease(5);

  EXPECT_EQ(5, decreased.getValue());
  EXPECT_EQ(10, quantity.getValue()); // 元のオブジェクトは不変
}

/**
 * @test 1減少できる（境界値）
 */
TEST_F(QuantityTest, CanDecreaseByOne) {
  Quantity quantity(10);
  Quantity decreased = quantity.decrease(1);

  EXPECT_EQ(9, decreased.getValue());
}

/**
 * @test 0まで減少できる（境界値）
 */
TEST_F(QuantityTest, CanDecreaseToZero) {
  Quantity quantity(10);
  Quantity decreased = quantity.decrease(10);

  EXPECT_EQ(0, decreased.getValue());
  EXPECT_TRUE(decreased.isZero());
}

/**
 * @test 1から1減らして0になる（境界値）
 */
TEST_F(QuantityTest, CanDecreaseFromOneToZero) {
  Quantity quantity(1);
  Quantity decreased = quantity.decrease(1);

  EXPECT_EQ(0, decreased.getValue());
  EXPECT_TRUE(decreased.isZero());
}

/**
 * @test 0を減少しても変わらない（境界値）
 */
TEST_F(QuantityTest, DecreaseByZeroDoesNotChange) {
  Quantity quantity(10);
  Quantity result = quantity.decrease(0);

  EXPECT_EQ(10, result.getValue());
}

/**
 * @test 0から減少しようとすると例外が発生する
 */
TEST_F(QuantityTest, ThrowsExceptionWhenDecreasingFromZero) {
  Quantity quantity(0);

  EXPECT_THROW({ quantity.decrease(1); }, std::domain_error);
}

/**
 * @test 在庫数を超えて減少しようとすると例外が発生する
 */
TEST_F(QuantityTest, ThrowsExceptionWhenDecreaseExceedsCurrentValue) {
  Quantity quantity(5);

  EXPECT_THROW({ quantity.decrease(10); }, std::domain_error);
}

/**
 * @test 負の値で減少しようとすると例外が発生する
 */
TEST_F(QuantityTest, ThrowsExceptionWhenDecreasingByNegativeAmount) {
  Quantity quantity(10);

  EXPECT_THROW({ quantity.decrease(-5); }, std::invalid_argument);
}

// ========================================
// 比較演算子テスト
// ========================================

/**
 * @test 等価演算子が正しく動作する
 */
TEST_F(QuantityTest, EqualityOperatorWorks) {
  Quantity quantity1(10);
  Quantity quantity2(10);
  Quantity quantity3(5);

  EXPECT_TRUE(quantity1 == quantity2);
  EXPECT_FALSE(quantity1 == quantity3);
}

/**
 * @test 不等価演算子が正しく動作する
 */
TEST_F(QuantityTest, InequalityOperatorWorks) {
  Quantity quantity1(10);
  Quantity quantity2(5);
  Quantity quantity3(10);

  EXPECT_TRUE(quantity1 != quantity2);
  EXPECT_FALSE(quantity1 != quantity3);
}

/**
 * @test 小なり演算子が正しく動作する
 */
TEST_F(QuantityTest, LessThanOperatorWorks) {
  Quantity quantity1(5);
  Quantity quantity2(10);
  Quantity quantity3(10);

  EXPECT_TRUE(quantity1 < quantity2);
  EXPECT_FALSE(quantity2 < quantity1);
  EXPECT_FALSE(quantity2 < quantity3);
}

/**
 * @test 大なり演算子が正しく動作する
 */
TEST_F(QuantityTest, GreaterThanOperatorWorks) {
  Quantity quantity1(10);
  Quantity quantity2(5);
  Quantity quantity3(10);

  EXPECT_TRUE(quantity1 > quantity2);
  EXPECT_FALSE(quantity2 > quantity1);
  EXPECT_FALSE(quantity1 > quantity3);
}

} // namespace test
} // namespace domain
} // namespace vending_machine
