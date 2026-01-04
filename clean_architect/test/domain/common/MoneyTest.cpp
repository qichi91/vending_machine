/**
 * @file MoneyTest.cpp
 * @brief Money Value Object のユニットテスト
 *
 * テスト方針:
 * - 同値分割法: 正の値、0、負の値（エラー）
 * - 境界値分析: 0、1、最大値付近
 * - 演算テスト: 加算、減算、比較
 */

#include "domain/common/Money.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

namespace vending_machine {
namespace domain {
namespace test {

/**
 * @brief Money Value Object の基本機能テスト
 */
class MoneyTest : public ::testing::Test {
protected:
  void SetUp() override {
    // テストごとの初期化が必要な場合
  }

  void TearDown() override {
    // テストごとのクリーンアップが必要な場合
  }
};

// ========================================
// 正常系テスト: 生成と取得
// ========================================

/**
 * @test 正の値でMoneyオブジェクトを生成できる
 */
TEST_F(MoneyTest, CanCreateWithPositiveValue) {
  Money money(100);
  EXPECT_EQ(100, money.getRawValue());
}

/**
 * @test 0円でMoneyオブジェクトを生成できる（境界値）
 */
TEST_F(MoneyTest, CanCreateWithZero) {
  Money money(0);
  EXPECT_EQ(0, money.getRawValue());
}

/**
 * @test 1円でMoneyオブジェクトを生成できる（境界値）
 */
TEST_F(MoneyTest, CanCreateWithOne) {
  Money money(1);
  EXPECT_EQ(1, money.getRawValue());
}

/**
 * @test 大きな金額でMoneyオブジェクトを生成できる
 */
TEST_F(MoneyTest, CanCreateWithLargeValue) {
  Money money(1000000);
  EXPECT_EQ(1000000, money.getRawValue());
}

// ========================================
// 異常系テスト: 不正な値
// ========================================

/**
 * @test 負の値でMoneyオブジェクトを生成しようとすると例外が発生する
 */
TEST_F(MoneyTest, ThrowsExceptionWhenCreatingWithNegativeValue) {
  EXPECT_THROW({ Money money(-1); }, std::invalid_argument);
}

/**
 * @test 負の値でMoneyオブジェクトを生成しようとすると例外が発生する（境界値）
 */
TEST_F(MoneyTest, ThrowsExceptionWhenCreatingWithMinusOne) {
  EXPECT_THROW({ Money money(-1); }, std::invalid_argument);
}

/**
 * @test 負の大きな値でMoneyオブジェクトを生成しようとすると例外が発生する
 */
TEST_F(MoneyTest, ThrowsExceptionWhenCreatingWithLargeNegativeValue) {
  EXPECT_THROW({ Money money(-1000); }, std::invalid_argument);
}

// ========================================
// 演算テスト: 加算
// ========================================

/**
 * @test 2つのMoneyオブジェクトを加算できる
 */
TEST_F(MoneyTest, CanAddTwoMoneyObjects) {
  Money money1(100);
  Money money2(50);
  Money result = money1 + money2;
  EXPECT_EQ(150, result.getRawValue());
}

/**
 * @test 0円との加算ができる（境界値）
 */
TEST_F(MoneyTest, CanAddZero) {
  Money money1(100);
  Money money2(0);
  Money result = money1 + money2;
  EXPECT_EQ(100, result.getRawValue());
}

/**
 * @test 加算後も元のオブジェクトは変更されない（不変性）
 */
TEST_F(MoneyTest, AdditionDoesNotModifyOriginalObjects) {
  Money money1(100);
  Money money2(50);
  Money result = money1 + money2;

  EXPECT_EQ(100, money1.getRawValue());
  EXPECT_EQ(50, money2.getRawValue());
  EXPECT_EQ(150, result.getRawValue());
}

// ========================================
// 演算テスト: 減算
// ========================================

/**
 * @test 2つのMoneyオブジェクトを減算できる
 */
TEST_F(MoneyTest, CanSubtractTwoMoneyObjects) {
  Money money1(100);
  Money money2(50);
  Money result = money1 - money2;
  EXPECT_EQ(50, result.getRawValue());
}

/**
 * @test 0円との減算ができる（境界値）
 */
TEST_F(MoneyTest, CanSubtractZero) {
  Money money1(100);
  Money money2(0);
  Money result = money1 - money2;
  EXPECT_EQ(100, result.getRawValue());
}

/**
 * @test 同額の減算で0円になる（境界値）
 */
TEST_F(MoneyTest, SubtractionResultsInZero) {
  Money money1(100);
  Money money2(100);
  Money result = money1 - money2;
  EXPECT_EQ(0, result.getRawValue());
}

/**
 * @test 減算結果が負になる場合は例外が発生する
 */
TEST_F(MoneyTest, ThrowsExceptionWhenSubtractionResultsInNegative) {
  Money money1(50);
  Money money2(100);
  EXPECT_THROW({ Money result = money1 - money2; }, std::domain_error);
}

/**
 * @test 減算後も元のオブジェクトは変更されない（不変性）
 */
TEST_F(MoneyTest, SubtractionDoesNotModifyOriginalObjects) {
  Money money1(100);
  Money money2(50);
  Money result = money1 - money2;

  EXPECT_EQ(100, money1.getRawValue());
  EXPECT_EQ(50, money2.getRawValue());
  EXPECT_EQ(50, result.getRawValue());
}

// ========================================
// 比較演算子テスト
// ========================================

/**
 * @test 等価演算子が正しく動作する
 */
TEST_F(MoneyTest, EqualityOperatorWorks) {
  Money money1(100);
  Money money2(100);
  Money money3(50);

  EXPECT_TRUE(money1 == money2);
  EXPECT_FALSE(money1 == money3);
}

/**
 * @test 不等価演算子が正しく動作する
 */
TEST_F(MoneyTest, InequalityOperatorWorks) {
  Money money1(100);
  Money money2(50);
  Money money3(100);

  EXPECT_TRUE(money1 != money2);
  EXPECT_FALSE(money1 != money3);
}

/**
 * @test 小なり演算子が正しく動作する
 */
TEST_F(MoneyTest, LessThanOperatorWorks) {
  Money money1(50);
  Money money2(100);
  Money money3(100);

  EXPECT_TRUE(money1 < money2);
  EXPECT_FALSE(money2 < money1);
  EXPECT_FALSE(money2 < money3);
}

/**
 * @test 小なりイコール演算子が正しく動作する
 */
TEST_F(MoneyTest, LessThanOrEqualOperatorWorks) {
  Money money1(50);
  Money money2(100);
  Money money3(100);

  EXPECT_TRUE(money1 <= money2);
  EXPECT_TRUE(money2 <= money3);
  EXPECT_FALSE(money2 <= money1);
}

/**
 * @test 大なり演算子が正しく動作する
 */
TEST_F(MoneyTest, GreaterThanOperatorWorks) {
  Money money1(100);
  Money money2(50);
  Money money3(100);

  EXPECT_TRUE(money1 > money2);
  EXPECT_FALSE(money2 > money1);
  EXPECT_FALSE(money1 > money3);
}

/**
 * @test 大なりイコール演算子が正しく動作する
 */
TEST_F(MoneyTest, GreaterThanOrEqualOperatorWorks) {
  Money money1(100);
  Money money2(50);
  Money money3(100);

  EXPECT_TRUE(money1 >= money2);
  EXPECT_TRUE(money1 >= money3);
  EXPECT_FALSE(money2 >= money1);
}

// ========================================
// その他のテスト
// ========================================

/**
 * @test 0円かどうかを判定できる
 */
TEST_F(MoneyTest, CanCheckIfZero) {
  Money zero(0);
  Money nonZero(100);

  EXPECT_TRUE(zero.isZero());
  EXPECT_FALSE(nonZero.isZero());
}

} // namespace test
} // namespace domain
} // namespace vending_machine
