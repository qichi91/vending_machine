#include "src/domain/sales/Sales.hpp"
#include "src/domain/inventory/SlotId.hpp"
#include "src/domain/sales/SalesId.hpp"
#include "src/domain/sales/SessionId.hpp"
#include <gtest/gtest.h>

using namespace vending_machine::domain;

class SalesTest : public ::testing::Test {};

// ========== 正常系テスト: 初期状態 ==========

TEST_F(SalesTest, CreateSales) {
  Sales sales(SalesId(1));

  EXPECT_EQ(1, sales.getId().getValue());
  EXPECT_EQ(Mode::NORMAL, sales.getMode());
  EXPECT_EQ(nullptr, sales.getCurrentSession());
}

// ========== セッション管理テスト ==========

TEST_F(SalesTest, StartSession) {
  Sales sales(SalesId(1));

  sales.startSession(SessionId(100));

  EXPECT_NE(nullptr, sales.getCurrentSession());
  EXPECT_EQ(100, sales.getCurrentSession()->getSessionId().getValue());
}

TEST_F(SalesTest, SelectProductInSession) {
  Sales sales(SalesId(1));
  sales.startSession(SessionId(100));

  sales.selectProduct(SlotId(5));

  EXPECT_TRUE(sales.getCurrentSession()->getSelectedSlotId().has_value());
  EXPECT_EQ(5, sales.getCurrentSession()->getSelectedSlotId()->getValue());
}

TEST_F(SalesTest, CompleteTransaction) {
  Sales sales(SalesId(1));
  sales.startSession(SessionId(100));
  sales.selectProduct(SlotId(5));
  sales.markPaymentPending();
  sales.markDispensing();

  sales.completeTransaction();

  EXPECT_EQ(nullptr, sales.getCurrentSession());
}

TEST_F(SalesTest, CancelTransaction) {
  Sales sales(SalesId(1));
  sales.startSession(SessionId(100));
  sales.selectProduct(SlotId(5));

  sales.cancelTransaction();

  EXPECT_EQ(nullptr, sales.getCurrentSession());
}

// ========== モード管理テスト ==========

TEST_F(SalesTest, StartMaintenance) {
  Sales sales(SalesId(1));

  sales.startMaintenance();

  EXPECT_EQ(Mode::MAINTENANCE, sales.getMode());
}

TEST_F(SalesTest, EndMaintenance) {
  Sales sales(SalesId(1));
  sales.startMaintenance();

  sales.endMaintenance();

  EXPECT_EQ(Mode::NORMAL, sales.getMode());
}

// ========== 異常系テスト ==========

TEST_F(SalesTest, CannotStartSessionInMaintenanceMode) {
  Sales sales(SalesId(1));
  sales.startMaintenance();

  EXPECT_THROW(sales.startSession(SessionId(100)), std::domain_error);
}

TEST_F(SalesTest, CannotStartSessionWhenSessionExists) {
  Sales sales(SalesId(1));
  sales.startSession(SessionId(100));

  EXPECT_THROW(sales.startSession(SessionId(101)), std::domain_error);
}

TEST_F(SalesTest, CannotSelectProductWithoutSession) {
  Sales sales(SalesId(1));

  EXPECT_THROW(sales.selectProduct(SlotId(5)), std::domain_error);
}

TEST_F(SalesTest, CannotStartMaintenanceWithActiveSession) {
  Sales sales(SalesId(1));
  sales.startSession(SessionId(100));
  sales.selectProduct(SlotId(5));

  EXPECT_THROW(sales.startMaintenance(), std::domain_error);
}

TEST_F(SalesTest, CanStartMaintenanceAfterSessionCompletes) {
  Sales sales(SalesId(1));
  sales.startSession(SessionId(100));
  sales.selectProduct(SlotId(5));
  sales.markPaymentPending();
  sales.markDispensing();
  sales.completeTransaction();

  EXPECT_NO_THROW(sales.startMaintenance());
  EXPECT_EQ(Mode::MAINTENANCE, sales.getMode());
}
