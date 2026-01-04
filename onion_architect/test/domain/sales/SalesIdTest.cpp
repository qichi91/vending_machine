#include "src/domain/sales/SalesId.hpp"
#include <gtest/gtest.h>

using namespace vending_machine::domain;

class SalesIdTest : public ::testing::Test {};

// ========== 正常系テスト ==========

TEST_F(SalesIdTest, ValidSalesId) {
  EXPECT_NO_THROW(SalesId(1));
  EXPECT_NO_THROW(SalesId(100));
}

TEST_F(SalesIdTest, GetValue) {
  SalesId id(5);
  EXPECT_EQ(5, id.getValue());
}

TEST_F(SalesIdTest, EqualityOperator) {
  SalesId id1(1);
  SalesId id2(1);
  SalesId id3(2);

  EXPECT_TRUE(id1 == id2);
  EXPECT_FALSE(id1 == id3);
}

TEST_F(SalesIdTest, InequalityOperator) {
  SalesId id1(1);
  SalesId id2(2);

  EXPECT_TRUE(id1 != id2);
  EXPECT_FALSE(id1 != SalesId(1));
}

// ========== 異常系テスト ==========

TEST_F(SalesIdTest, ZeroThrowsException) {
  EXPECT_THROW(SalesId(0), std::invalid_argument);
}

TEST_F(SalesIdTest, NegativeThrowsException) {
  EXPECT_THROW(SalesId(-1), std::invalid_argument);
}
