#include "domain/Money.hpp"
#include <gtest/gtest.h>

namespace vending_machine::domain::test {

class MoneyTest : public ::testing::Test {};

// 正常系: 任意の非負整数が許可される
TEST_F(MoneyTest, ShouldCreateMoneyWithAnyPositiveAmounts) {
  EXPECT_NO_THROW(Money(10));
  EXPECT_NO_THROW(Money(1)); // Valid Money, Invalid Denomination
  EXPECT_NO_THROW(Money(120));
}

// 異常系: 負の数は例外
TEST_F(MoneyTest, ShouldThrowExceptionForNegativeAmounts) {
  EXPECT_THROW(Money(-1), std::invalid_argument);
  EXPECT_THROW(Money(-100), std::invalid_argument);
}

// 金種判定のテスト
TEST_F(MoneyTest, ShouldIdentifyValidDenominations) {
  EXPECT_TRUE(Money(10).isValidDenomination());
  EXPECT_TRUE(Money(50).isValidDenomination());
  EXPECT_TRUE(Money(100).isValidDenomination());
  EXPECT_TRUE(Money(500).isValidDenomination());
  EXPECT_TRUE(Money(1000).isValidDenomination());

  EXPECT_FALSE(Money(1).isValidDenomination());
  EXPECT_FALSE(Money(5).isValidDenomination());
  EXPECT_FALSE(Money(11).isValidDenomination());
  EXPECT_FALSE(Money(2000).isValidDenomination()); // 今回の仕様では1000円まで
}

// 演算のテスト: 加算
TEST_F(MoneyTest, ShouldAddMoneyValues) {
  Money m1(100);
  Money m2(50);
  Money result = m1 + m2; // 150

  EXPECT_EQ(result.amount(), 150);
  EXPECT_FALSE(result.isValidDenomination()); // 150円硬貨はない
}

// 比較のテスト
TEST_F(MoneyTest, ShouldCompareMoneyValues) {
  Money m1(100);
  Money m2(100);
  Money m3(500);

  EXPECT_EQ(m1, m2);
  EXPECT_NE(m1, m3);
  EXPECT_LT(m1, m3);
}

} // namespace vending_machine::domain::test
