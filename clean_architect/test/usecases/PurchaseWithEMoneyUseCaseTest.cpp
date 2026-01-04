#include "usecases/usecases/PurchaseWithEMoneyUseCase.hpp"
#include "domain/common/Money.hpp"
#include "domain/common/Price.hpp"
#include "domain/common/Quantity.hpp"
#include "domain/interfaces/IDispenser.hpp"
#include "domain/interfaces/IPaymentGateway.hpp"
#include "domain/inventory/Inventory.hpp"
#include "domain/inventory/ProductInfo.hpp"
#include "domain/inventory/ProductName.hpp"
#include "domain/inventory/ProductSlot.hpp"
#include "domain/payment/Wallet.hpp"
#include "domain/repositories/ITransactionHistoryRepository.hpp"
#include "domain/sales/Sales.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace vending_machine {
namespace usecases {

// Mockクラス
class MockPaymentGateway : public domain::IPaymentGateway {
public:
  MOCK_METHOD(void, requestPayment, (const domain::Price &amount), (override));
  MOCK_METHOD(void, cancelPayment, (), (override));
  MOCK_METHOD(domain::PaymentStatus, getPaymentStatus, (), (const override));
};

class MockDispenser : public domain::IDispenser {
public:
  MOCK_METHOD(void, dispense, (const domain::ProductInfo &product), (override));
  MOCK_METHOD(bool, canDispense, (const domain::ProductInfo &product),
              (const override));
};

class MockTransactionHistoryRepository
    : public domain::ITransactionHistoryRepository {
public:
  MOCK_METHOD(void, save, (const domain::TransactionRecord &record),
              (override));
  MOCK_METHOD(std::vector<domain::TransactionRecord>, getAll, (),
              (const, override));
  MOCK_METHOD(std::vector<domain::TransactionRecord>, getBySlotId,
              (const domain::SlotId &slot_id), (const, override));
  MOCK_METHOD(domain::Money, getTotalRevenue, (), (const, override));
  MOCK_METHOD(void, clear, (), (override));
};

class PurchaseWithEMoneyUseCaseTest : public ::testing::Test {
protected:
  void SetUp() override {
    // 在庫のセットアップ
    domain::ProductSlot slot1(
        domain::SlotId(1),
        domain::ProductInfo(domain::ProductName("Coke"), domain::Price(120)),
        domain::Quantity(0));
    inventory.addSlot(slot1);
    inventory.refill(domain::SlotId(1), domain::Quantity(10));

    domain::ProductSlot slot2(
        domain::SlotId(2),
        domain::ProductInfo(domain::ProductName("Pepsi"), domain::Price(150)),
        domain::Quantity(0));
    inventory.addSlot(slot2);
    inventory.refill(domain::SlotId(2), domain::Quantity(5));

    domain::ProductSlot slot3(
        domain::SlotId(3),
        domain::ProductInfo(domain::ProductName("Water"), domain::Price(100)),
        domain::Quantity(0));
    inventory.addSlot(slot3);
    // slot3は在庫0のまま

    // Salesのセットアップ
    sales = std::make_unique<domain::Sales>(domain::SalesId(1));

    // UseCaseの生成
    use_case = std::make_unique<PurchaseWithEMoneyUseCase>(
        inventory, wallet, *sales, mock_payment_gateway, mock_dispenser,
        mock_repository);
  }

  domain::Inventory inventory;
  domain::Wallet wallet;
  std::unique_ptr<domain::Sales> sales;
  MockPaymentGateway mock_payment_gateway;
  MockDispenser mock_dispenser;
  MockTransactionHistoryRepository mock_repository;
  std::unique_ptr<PurchaseWithEMoneyUseCase> use_case;
};

// テスト1: セッション開始
TEST_F(PurchaseWithEMoneyUseCaseTest, CanStartSession) {
  EXPECT_NO_THROW(use_case->startSession());
}

// テスト2: 在庫がある商品のみ取得（残高チェックなし）
TEST_F(PurchaseWithEMoneyUseCaseTest, GetAvailableProductsReturnsInStockOnly) {
  use_case->startSession();

  auto available = use_case->getAvailableProducts();

  // スロット1と2のみ（スロット3は在庫0）
  ASSERT_EQ(2, available.size());

  std::vector<domain::SlotId> slot_ids;
  for (const auto &product : available) {
    slot_ids.push_back(product.getSlotId());
  }

  EXPECT_TRUE(std::find(slot_ids.begin(), slot_ids.end(), domain::SlotId(1)) !=
              slot_ids.end());
  EXPECT_TRUE(std::find(slot_ids.begin(), slot_ids.end(), domain::SlotId(2)) !=
              slot_ids.end());
  EXPECT_TRUE(std::find(slot_ids.begin(), slot_ids.end(), domain::SlotId(3)) ==
              slot_ids.end());
}

// テスト3: 決済成功時の購入フロー
TEST_F(PurchaseWithEMoneyUseCaseTest, SuccessfulPurchaseWithAuthorizedPayment) {
  use_case->startSession();

  // 決済ゲートウェイのMock設定
  EXPECT_CALL(mock_payment_gateway, requestPayment(domain::Price(120)))
      .Times(1);
  EXPECT_CALL(mock_payment_gateway, getPaymentStatus())
      .WillOnce(::testing::Return(domain::PaymentStatus::Authorized));
  EXPECT_CALL(mock_dispenser, dispense(testing::_)).Times(1);

  bool result = use_case->selectAndRequestPayment(domain::SlotId(1));

  EXPECT_TRUE(result);
  // 在庫が減っている
  EXPECT_EQ(9, inventory.getSlot(domain::SlotId(1)).getStock().getValue());
  // Walletの残高は0（決済後即座に引き落とし）
  EXPECT_EQ(0, wallet.getBalance().getRawValue());
}

// テスト4: 決済失敗時のフロー
TEST_F(PurchaseWithEMoneyUseCaseTest, FailedPurchaseWithFailedPayment) {
  use_case->startSession();

  EXPECT_CALL(mock_payment_gateway, requestPayment(domain::Price(120)))
      .Times(1);
  EXPECT_CALL(mock_payment_gateway, getPaymentStatus())
      .WillOnce(::testing::Return(domain::PaymentStatus::Failed));
  // ディスペンサーは呼ばれない
  EXPECT_CALL(mock_dispenser, dispense).Times(0);

  bool result = use_case->selectAndRequestPayment(domain::SlotId(1));

  EXPECT_FALSE(result);
  // 在庫は減っていない
  EXPECT_EQ(10, inventory.getSlot(domain::SlotId(1)).getStock().getValue());
}

// テスト5: 決済キャンセル時のフロー
TEST_F(PurchaseWithEMoneyUseCaseTest, CancelledPurchase) {
  use_case->startSession();

  EXPECT_CALL(mock_payment_gateway, requestPayment(domain::Price(120)))
      .Times(1);
  EXPECT_CALL(mock_payment_gateway, getPaymentStatus())
      .WillOnce(::testing::Return(domain::PaymentStatus::Cancelled));
  EXPECT_CALL(mock_dispenser, dispense).Times(0);

  bool result = use_case->selectAndRequestPayment(domain::SlotId(1));

  EXPECT_FALSE(result);
  EXPECT_EQ(10, inventory.getSlot(domain::SlotId(1)).getStock().getValue());
}

// テスト6: 決済キャンセルメソッド（セッションのみキャンセル）
TEST_F(PurchaseWithEMoneyUseCaseTest, CanCancelPayment) {
  use_case->startSession();

  // pending_price_がない場合、cancelPaymentは呼ばれない
  EXPECT_CALL(mock_payment_gateway, cancelPayment()).Times(0);

  EXPECT_NO_THROW(use_case->cancelPayment());
}

// テスト7: 在庫なし商品は購入不可
TEST_F(PurchaseWithEMoneyUseCaseTest, CannotPurchaseOutOfStockProduct) {
  use_case->startSession();

  // スロット3は在庫0
  EXPECT_THROW(use_case->selectAndRequestPayment(domain::SlotId(3)),
               std::domain_error);
}

// テスト8: 複数商品の連続購入
TEST_F(PurchaseWithEMoneyUseCaseTest, CanPurchaseMultipleProducts) {
  // 1回目
  use_case->startSession();
  EXPECT_CALL(mock_payment_gateway, requestPayment(domain::Price(120)))
      .Times(1);
  EXPECT_CALL(mock_payment_gateway, getPaymentStatus())
      .WillOnce(::testing::Return(domain::PaymentStatus::Authorized));
  EXPECT_CALL(mock_dispenser, dispense(testing::_)).Times(1);

  bool result1 = use_case->selectAndRequestPayment(domain::SlotId(1));
  EXPECT_TRUE(result1);

  // 2回目
  use_case->startSession();
  EXPECT_CALL(mock_payment_gateway, requestPayment(domain::Price(150)))
      .Times(1);
  EXPECT_CALL(mock_payment_gateway, getPaymentStatus())
      .WillOnce(::testing::Return(domain::PaymentStatus::Authorized));
  EXPECT_CALL(mock_dispenser, dispense(testing::_)).Times(1);

  bool result2 = use_case->selectAndRequestPayment(domain::SlotId(2));
  EXPECT_TRUE(result2);

  // 在庫確認
  EXPECT_EQ(9, inventory.getSlot(domain::SlotId(1)).getStock().getValue());
  EXPECT_EQ(4, inventory.getSlot(domain::SlotId(2)).getStock().getValue());
}

// テスト9: 決済PENDING状態の処理
TEST_F(PurchaseWithEMoneyUseCaseTest, HandlesPendingPaymentStatus) {
  use_case->startSession();

  EXPECT_CALL(mock_payment_gateway, requestPayment(domain::Price(120)))
      .Times(1);
  EXPECT_CALL(mock_payment_gateway, getPaymentStatus())
      .WillOnce(::testing::Return(domain::PaymentStatus::Pending));
  EXPECT_CALL(mock_dispenser, dispense).Times(0);

  bool result = use_case->selectAndRequestPayment(domain::SlotId(1));

  // PENDING状態では購入失敗として扱う
  EXPECT_FALSE(result);
  EXPECT_EQ(10, inventory.getSlot(domain::SlotId(1)).getStock().getValue());
}

// テスト10: 高額商品も購入可能（残高チェックなし）
TEST_F(PurchaseWithEMoneyUseCaseTest,
       CanPurchaseExpensiveProductWithoutBalanceCheck) {
  use_case->startSession();

  // Walletは残高0だが、電子決済では問題ない
  EXPECT_EQ(0, wallet.getBalance().getRawValue());

  auto available = use_case->getAvailableProducts();

  // 在庫があればすべて購入可能
  ASSERT_EQ(2, available.size());

  EXPECT_CALL(mock_payment_gateway, requestPayment(domain::Price(150)))
      .Times(1);
  EXPECT_CALL(mock_payment_gateway, getPaymentStatus())
      .WillOnce(::testing::Return(domain::PaymentStatus::Authorized));
  EXPECT_CALL(mock_dispenser, dispense(testing::_)).Times(1);

  bool result = use_case->selectAndRequestPayment(domain::SlotId(2));
  EXPECT_TRUE(result);
}

} // namespace usecases
} // namespace vending_machine
