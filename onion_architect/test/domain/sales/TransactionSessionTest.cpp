#include "src/domain/sales/TransactionSession.hpp"
#include "src/domain/inventory/SlotId.hpp"
#include "src/domain/sales/SessionId.hpp"
#include <gtest/gtest.h>

using namespace vending_machine::domain;

class TransactionSessionTest : public ::testing::Test {};

// ========== 正常系テスト ==========

TEST_F(TransactionSessionTest, CreateSession) {
  SessionId id(1);
  TransactionSession session(id);

  EXPECT_EQ(1, session.getSessionId().getValue());
  EXPECT_EQ(SessionStatus::PRODUCT_SELECTING, session.getStatus());
  EXPECT_FALSE(session.getSelectedSlotId().has_value());
}

TEST_F(TransactionSessionTest, SelectProduct) {
  TransactionSession session(SessionId(1));
  SlotId slot_id(5);

  session.selectProduct(slot_id);

  EXPECT_TRUE(session.getSelectedSlotId().has_value());
  EXPECT_EQ(5, session.getSelectedSlotId()->getValue());
}

TEST_F(TransactionSessionTest, MarkPaymentPending) {
  TransactionSession session(SessionId(1));
  session.selectProduct(SlotId(5));

  session.markPaymentPending();

  EXPECT_EQ(SessionStatus::PAYMENT_PENDING, session.getStatus());
}

TEST_F(TransactionSessionTest, MarkDispensing) {
  TransactionSession session(SessionId(1));
  session.selectProduct(SlotId(5));
  session.markPaymentPending();

  session.markDispensing();

  EXPECT_EQ(SessionStatus::DISPENSING, session.getStatus());
}

TEST_F(TransactionSessionTest, Complete) {
  TransactionSession session(SessionId(1));
  session.selectProduct(SlotId(5));
  session.markPaymentPending();
  session.markDispensing();

  session.complete();

  EXPECT_EQ(SessionStatus::COMPLETED, session.getStatus());
  EXPECT_TRUE(session.isFinished());
}

TEST_F(TransactionSessionTest, Cancel) {
  TransactionSession session(SessionId(1));
  session.selectProduct(SlotId(5));

  session.cancel();

  EXPECT_EQ(SessionStatus::CANCELLED, session.getStatus());
  EXPECT_TRUE(session.isFinished());
}

// ========== 異常系テスト ==========

TEST_F(TransactionSessionTest, CannotSelectProductTwice) {
  TransactionSession session(SessionId(1));
  session.selectProduct(SlotId(5));

  EXPECT_THROW(session.selectProduct(SlotId(6)), std::domain_error);
}

TEST_F(TransactionSessionTest, CannotMarkPaymentPendingWithoutProduct) {
  TransactionSession session(SessionId(1));

  EXPECT_THROW(session.markPaymentPending(), std::domain_error);
}

TEST_F(TransactionSessionTest, CannotMarkDispensingWithoutPaymentPending) {
  TransactionSession session(SessionId(1));
  session.selectProduct(SlotId(5));

  EXPECT_THROW(session.markDispensing(), std::domain_error);
}

TEST_F(TransactionSessionTest, CannotCompleteWithoutDispensing) {
  TransactionSession session(SessionId(1));
  session.selectProduct(SlotId(5));
  session.markPaymentPending();

  EXPECT_THROW(session.complete(), std::domain_error);
}
