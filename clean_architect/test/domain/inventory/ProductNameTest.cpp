#include "domain/inventory/ProductName.hpp"
#include <gtest/gtest.h>

using namespace vending_machine::domain;

/**
 * @class ProductNameTest
 * @brief ProductName値オブジェクトのテストクラス
 */
class ProductNameTest : public ::testing::Test {};

// ========== 正常系テスト ==========

/**
 * @brief 有効な商品名でProductNameを作成できることを確認
 */
TEST_F(ProductNameTest, ValidProductName) {
  EXPECT_NO_THROW(ProductName("Coke"));
  EXPECT_NO_THROW(ProductName("Pepsi"));
  EXPECT_NO_THROW(ProductName("Water"));
  EXPECT_NO_THROW(ProductName("A"));
  EXPECT_NO_THROW(ProductName("Very Long Product Name With Spaces"));
}

/**
 * @brief getValue()が正しい商品名を返すことを確認
 */
TEST_F(ProductNameTest, GetValue) {
  ProductName name("Coke");
  EXPECT_EQ("Coke", name.getValue());

  ProductName name2("Green Tea");
  EXPECT_EQ("Green Tea", name2.getValue());
}

/**
 * @brief 等価比較演算子が正しく動作することを確認
 */
TEST_F(ProductNameTest, EqualityOperator) {
  ProductName name1("Coke");
  ProductName name2("Coke");
  ProductName name3("Pepsi");

  EXPECT_TRUE(name1 == name2);
  EXPECT_FALSE(name1 == name3);
}

/**
 * @brief 不等価比較演算子が正しく動作することを確認
 */
TEST_F(ProductNameTest, InequalityOperator) {
  ProductName name1("Coke");
  ProductName name2("Pepsi");
  ProductName name3("Coke");

  EXPECT_TRUE(name1 != name2);
  EXPECT_FALSE(name1 != name3);
}

// ========== 異常系テスト ==========

/**
 * @brief 空文字列でProductNameを作成しようとすると例外がスローされることを確認
 */
TEST_F(ProductNameTest, EmptyNameThrowsException) {
  EXPECT_THROW(ProductName(""), std::invalid_argument);
}

/**
 * @brief 空文字列の例外メッセージが適切であることを確認
 */
TEST_F(ProductNameTest, EmptyNameExceptionMessage) {
  try {
    ProductName("");
    FAIL() << "Expected std::invalid_argument";
  } catch (const std::invalid_argument &e) {
    EXPECT_STREQ("Product name cannot be empty", e.what());
  }
}

// ========== 境界値分析テスト ==========

/**
 * @brief 1文字の商品名でProductNameを作成できることを確認（最小有効値）
 */
TEST_F(ProductNameTest, SingleCharacterName) {
  ProductName name("A");
  EXPECT_EQ("A", name.getValue());
}

/**
 * @brief スペースを含む商品名を扱えることを確認
 */
TEST_F(ProductNameTest, NameWithSpaces) {
  ProductName name("Coca Cola");
  EXPECT_EQ("Coca Cola", name.getValue());
}

/**
 * @brief 特殊文字を含む商品名を扱えることを確認
 */
TEST_F(ProductNameTest, NameWithSpecialCharacters) {
  ProductName name("C&C Lemon");
  EXPECT_EQ("C&C Lemon", name.getValue());

  ProductName name2("7-Up");
  EXPECT_EQ("7-Up", name2.getValue());

  ProductName name3("100% Juice");
  EXPECT_EQ("100% Juice", name3.getValue());
}
