/**
 * @file PriceTest.cpp
 * @brief Price Value Object のユニットテスト
 *
 * テスト方針:
 * - 同値分割法: 正の値、0（無料商品）、負の値（エラー）
 * - 境界値分析: 0、1、自販機の標準価格帯（50円〜500円）
 * - Money型との相互運用性
 */

#include "domain/common/Price.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

namespace vending_machine {
namespace domain {
namespace test {

/**
 * @brief Price Value Object の基本機能テスト
 */
class PriceTest : public ::testing::Test {
protected:
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
 * @test 正の値でPriceオブジェクトを生成できる
 */
TEST_F(PriceTest, CanCreateWithPositiveValue) {
  Price price(100);
  EXPECT_EQ(100, price.getRawValue());
}

/**
 * @test 0円（無料）でPriceオブジェクトを生成できる
 */
TEST_F(PriceTest, CanCreateWithZero) {
  Price price(0);
  EXPECT_EQ(0, price.getRawValue());
}

/**
 * @test 1円でPriceオブジェクトを生成できる（境界値）
 */
TEST_F(PriceTest, CanCreateWithOne) {
  Price price(1);
  EXPECT_EQ(1, price.getRawValue());
}

/**
 * @test 標準的な自販機価格でPriceオブジェクトを生成できる
 */
TEST_F(PriceTest, CanCreateWithStandardVendingPrice) {
  Price price50(50);
  Price price100(100);
  Price price120(120);
  Price price150(150);
  Price price500(500);

  EXPECT_EQ(50, price50.getRawValue());
  EXPECT_EQ(100, price100.getRawValue());
  EXPECT_EQ(120, price120.getRawValue());
  EXPECT_EQ(150, price150.getRawValue());
  EXPECT_EQ(500, price500.getRawValue());
}

// ========================================
// 異常系テスト: 不正な値
// ========================================

/**
 * @test 負の値でPriceオブジェクトを生成しようとすると例外が発生する
 */
TEST_F(PriceTest, ThrowsExceptionWhenCreatingWithNegativeValue) {
  EXPECT_THROW({ Price price(-1); }, std::invalid_argument);
}

/**
 * @test 負の大きな値でPriceオブジェクトを生成しようとすると例外が発生する
 */
TEST_F(PriceTest, ThrowsExceptionWhenCreatingWithLargeNegativeValue) {
  EXPECT_THROW({ Price price(-1000); }, std::invalid_argument);
}

// ========================================
// 比較演算子テスト
// ========================================

/**
 * @test 等価演算子が正しく動作する
 */
TEST_F(PriceTest, EqualityOperatorWorks) {
  Price price1(100);
  Price price2(100);
  Price price3(150);

  EXPECT_TRUE(price1 == price2);
  EXPECT_FALSE(price1 == price3);
}

/**
 * @test 不等価演算子が正しく動作する
 */
TEST_F(PriceTest, InequalityOperatorWorks) {
  Price price1(100);
  Price price2(150);
  Price price3(100);

  EXPECT_TRUE(price1 != price2);
  EXPECT_FALSE(price1 != price3);
}

/**
 * @test 小なり演算子が正しく動作する
 */
TEST_F(PriceTest, LessThanOperatorWorks) {
  Price price1(100);
  Price price2(150);
  Price price3(150);

  EXPECT_TRUE(price1 < price2);
  EXPECT_FALSE(price2 < price1);
  EXPECT_FALSE(price2 < price3);
}

/**
 * @test 小なりイコール演算子が正しく動作する
 */
TEST_F(PriceTest, LessThanOrEqualOperatorWorks) {
  Price price1(100);
  Price price2(150);
  Price price3(150);

  EXPECT_TRUE(price1 <= price2);
  EXPECT_TRUE(price2 <= price3);
  EXPECT_FALSE(price2 <= price1);
}

/**
 * @test 大なり演算子が正しく動作する
 */
TEST_F(PriceTest, GreaterThanOperatorWorks) {
  Price price1(150);
  Price price2(100);
  Price price3(150);

  EXPECT_TRUE(price1 > price2);
  EXPECT_FALSE(price2 > price1);
  EXPECT_FALSE(price1 > price3);
}

/**
 * @test 大なりイコール演算子が正しく動作する
 */
TEST_F(PriceTest, GreaterThanOrEqualOperatorWorks) {
  Price price1(150);
  Price price2(100);
  Price price3(150);

  EXPECT_TRUE(price1 >= price2);
  EXPECT_TRUE(price1 >= price3);
  EXPECT_FALSE(price2 >= price1);
}

} // namespace test
} // namespace domain
} // namespace vending_machine
