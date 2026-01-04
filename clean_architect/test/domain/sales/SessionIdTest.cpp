#include "domain/sales/SessionId.hpp"
#include <gtest/gtest.h>

using namespace vending_machine::domain;

class SessionIdTest : public ::testing::Test {};

// ========== 正常系テスト ==========

TEST_F(SessionIdTest, ValidSessionId) {
  EXPECT_NO_THROW(SessionId(1));
  EXPECT_NO_THROW(SessionId(100));
}

TEST_F(SessionIdTest, GetValue) {
  SessionId id(5);
  EXPECT_EQ(5, id.getValue());
}

TEST_F(SessionIdTest, EqualityOperator) {
  SessionId id1(1);
  SessionId id2(1);
  SessionId id3(2);

  EXPECT_TRUE(id1 == id2);
  EXPECT_FALSE(id1 == id3);
}

TEST_F(SessionIdTest, InequalityOperator) {
  SessionId id1(1);
  SessionId id2(2);

  EXPECT_TRUE(id1 != id2);
  EXPECT_FALSE(id1 != SessionId(1));
}

// ========== 異常系テスト ==========

TEST_F(SessionIdTest, ZeroThrowsException) {
  EXPECT_THROW(SessionId(0), std::invalid_argument);
}

TEST_F(SessionIdTest, NegativeThrowsException) {
  EXPECT_THROW(SessionId(-1), std::invalid_argument);
}
