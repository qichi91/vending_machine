/**
 * @file ProductInfoTest.cpp
 * @brief ProductInfo Value Object のユニットテスト
 *
 * テスト方針:
 * - 商品情報の不変性
 * - 名前と価格の完全性
 */

#include "src/domain/inventory/ProductInfo.hpp"
#include "src/domain/common/Price.hpp"
#include "src/domain/inventory/ProductName.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

namespace vending_machine {
namespace domain {
namespace test {

class ProductInfoTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

// ========================================
// 正常系テスト: 生成と取得
// ========================================

/**
 * @test 商品名と価格でProductInfoオブジェクトを生成できる
 */
TEST_F(ProductInfoTest, CanCreateWithNameAndPrice) {
  ProductName name("Cola");
  Price price(100);
  ProductInfo product_info(name, price);

  EXPECT_EQ("Cola", product_info.getName().getValue());
  EXPECT_EQ(100, product_info.getPrice().getRawValue());
}

/**
 * @test 異なる商品名で生成できる
 */
TEST_F(ProductInfoTest, CanCreateWithDifferentNames) {
  ProductName name1("Cola");
  Price price1(100);
  ProductInfo cola(name1, price1);

  ProductName name2("Coffee");
  Price price2(120);
  ProductInfo coffee(name2, price2);

  ProductName name3("Juice");
  Price price3(150);
  ProductInfo juice(name3, price3);

  EXPECT_EQ("Cola", cola.getName().getValue());
  EXPECT_EQ("Coffee", coffee.getName().getValue());
  EXPECT_EQ("Juice", juice.getName().getValue());
}

/**
 * @test 異なる価格で生成できる
 */
TEST_F(ProductInfoTest, CanCreateWithDifferentPrices) {
  ProductName name1("Item1");
  Price price1(50);
  ProductInfo product1(name1, price1);

  ProductName name2("Item2");
  Price price2(100);
  ProductInfo product2(name2, price2);

  ProductName name3("Item3");
  Price price3(200);
  ProductInfo product3(name3, price3);

  EXPECT_EQ(50, product1.getPrice().getRawValue());
  EXPECT_EQ(100, product2.getPrice().getRawValue());
  EXPECT_EQ(200, product3.getPrice().getRawValue());
}

/**
 * @test 0円（無料）の商品を生成できる
 */
TEST_F(ProductInfoTest, CanCreateWithZeroPrice) {
  ProductName name("Free Item");
  Price price(0);
  ProductInfo product_info(name, price);

  EXPECT_EQ(0, product_info.getPrice().getRawValue());
}

// ========================================
// 不変性テスト
// ========================================

/**
 * @test 生成後、オブジェクトの値は変更されない
 */
TEST_F(ProductInfoTest, ObjectIsImmutable) {
  ProductName original_name("Original");
  Price original_price(100);
  ProductInfo product_info(original_name, original_price);

  ProductName modified_name("Modified");
  Price modified_price(200);
  ProductInfo modified_product(modified_name, modified_price);

  // 元のオブジェクトは変更されていない
  EXPECT_EQ("Original", product_info.getName().getValue());
  EXPECT_EQ(100, product_info.getPrice().getRawValue());
}

// ========================================
// 等価性テスト
// ========================================

/**
 * @test 同じ名前と価格のProductInfoは等しい
 */
TEST_F(ProductInfoTest, EqualityOperatorWorks) {
  ProductName name1("Cola");
  Price price1(100);
  ProductInfo product1(name1, price1);

  ProductName name2("Cola");
  Price price2(100);
  ProductInfo product2(name2, price2);

  ProductName name3("Coffee");
  Price price3(150);
  ProductInfo product3(name3, price3);

  EXPECT_TRUE(product1 == product2);
  EXPECT_FALSE(product1 == product3);
}

/**
 * @test 名前が同じでも価格が異なると等しくない
 */
TEST_F(ProductInfoTest, DifferentPricesMakesInequal) {
  ProductName name1("Cola");
  Price price1(100);
  ProductInfo product1(name1, price1);

  ProductName name2("Cola");
  Price price2(120);
  ProductInfo product2(name2, price2);

  EXPECT_FALSE(product1 == product2);
  EXPECT_TRUE(product1 != product2);
}

/**
 * @test 価格が同じでも名前が異なると等しくない
 */
TEST_F(ProductInfoTest, DifferentNamesMakesInequal) {
  ProductName name1("Cola");
  ProductName name2("Cider");
  Price price(100);
  ProductInfo product1(name1, price);
  ProductInfo product2(name2, price);

  EXPECT_FALSE(product1 == product2);
  EXPECT_TRUE(product1 != product2);
}

/**
 * @test 不等価演算子が正しく動作する
 */
TEST_F(ProductInfoTest, InequalityOperatorWorks) {
  ProductName name1("Cola");
  Price price1(100);
  ProductInfo product1(name1, price1);

  ProductName name2("Cola");
  Price price2(100);
  ProductInfo product2(name2, price2);

  ProductName name3("Coffee");
  Price price3(150);
  ProductInfo product3(name3, price3);

  EXPECT_FALSE(product1 != product2);
  EXPECT_TRUE(product1 != product3);
}

} // namespace test
} // namespace domain
} // namespace vending_machine
