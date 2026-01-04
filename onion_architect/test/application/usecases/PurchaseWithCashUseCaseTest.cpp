#include "src/application/usecases/PurchaseWithCashUseCase.hpp"
#include "src/domain/common/Money.hpp"
#include "src/domain/common/Price.hpp"
#include "src/domain/common/Quantity.hpp"
#include "src/domain/interfaces/ICoinMech.hpp"
#include "src/domain/interfaces/IDispenser.hpp"
#include "src/domain/inventory/Inventory.hpp"
#include "src/domain/inventory/ProductInfo.hpp"
#include "src/domain/inventory/ProductName.hpp"
#include "src/domain/inventory/ProductSlot.hpp"
#include "src/domain/payment/Wallet.hpp"
#include "src/domain/repositories/ITransactionHistoryRepository.hpp"
#include "src/domain/sales/Sales.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace vending_machine {
namespace application {

// Mockクラス
class MockCoinMech : public domain::ICoinMech {
public:
  MOCK_METHOD(bool, canMakeChange, (const domain::Money &amount),
              (const override));
  MOCK_METHOD(void, dispense, (const domain::Money &amount), (override));
};

class MockDispenser : public domain::IDispenser {
public:
  MOCK_METHOD(bool, canDispense, (const domain::ProductInfo &product),
              (const override));
  MOCK_METHOD(void, dispense, (const domain::ProductInfo &product), (override));
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

class PurchaseWithCashUseCaseTest : public ::testing::Test {
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

    // Salesのセットアップ（通常モード）
    sales = std::make_unique<domain::Sales>(domain::SalesId(1));

    // UseCaseの生成
    use_case = std::make_unique<PurchaseWithCashUseCase>(
        inventory, wallet, *sales, mock_coin_mech, mock_dispenser,
        mock_repository);
  }

  domain::Inventory inventory;
  domain::Wallet wallet;
  std::unique_ptr<domain::Sales> sales;
  MockCoinMech mock_coin_mech;
  MockDispenser mock_dispenser;
  MockTransactionHistoryRepository mock_repository;
  std::unique_ptr<PurchaseWithCashUseCase> use_case;
};

// テスト1: セッション開始
TEST_F(PurchaseWithCashUseCaseTest, CanStartSession) {
  EXPECT_NO_THROW(use_case->startSession());
}

// テスト2: 現金投入
TEST_F(PurchaseWithCashUseCaseTest, CanInsertCash) {
  use_case->startSession();
  use_case->insertCash(domain::Money(500));

  EXPECT_EQ(500, use_case->getBalance().getRawValue());
}

// テスト3: 購入可能商品の取得
TEST_F(PurchaseWithCashUseCaseTest, CanGetEligibleProducts) {
  use_case->startSession();
  use_case->insertCash(domain::Money(200));

  // すべての商品で釣銭準備OK
  ON_CALL(mock_coin_mech, canMakeChange).WillByDefault(::testing::Return(true));

  auto eligible = use_case->getEligibleProducts();

  // 価格120円、150円の両方が購入可能
  ASSERT_EQ(2, eligible.size());
}

// テスト4: 残高不足の場合、購入可能商品は減る
TEST_F(PurchaseWithCashUseCaseTest, EligibleProductsWithInsufficientBalance) {
  use_case->startSession();
  use_case->insertCash(domain::Money(100)); // 120円未満

  ON_CALL(mock_coin_mech, canMakeChange).WillByDefault(::testing::Return(true));

  auto eligible = use_case->getEligibleProducts();

  // 購入可能な商品なし
  ASSERT_EQ(0, eligible.size());
}

// テスト5: 商品購入（ハッピーパス）
TEST_F(PurchaseWithCashUseCaseTest, CanPurchaseProduct) {
  use_case->startSession();
  use_case->insertCash(domain::Money(500));

  // ディスペンサーとコインメックの期待値設定
  EXPECT_CALL(mock_dispenser, dispense(testing::_)).Times(1);
  EXPECT_CALL(mock_coin_mech, dispense(domain::Money(380))) // 500 - 120
      .Times(1);

  use_case->selectAndPurchase(domain::SlotId(1));

  // 残高が0になっている
  EXPECT_EQ(0, use_case->getBalance().getRawValue());

  // 在庫が減っている
  EXPECT_EQ(9, inventory.getSlot(domain::SlotId(1)).getStock().getValue());
}

// テスト6: ちょうどの金額で購入（お釣りなし）
TEST_F(PurchaseWithCashUseCaseTest, PurchaseWithExactAmount) {
  use_case->startSession();
  use_case->insertCash(domain::Money(120)); // ちょうど120円

  // ディスペンサーは呼ばれる、コインメックは呼ばれない
  EXPECT_CALL(mock_dispenser, dispense(testing::_)).Times(1);
  EXPECT_CALL(mock_coin_mech, dispense).Times(0);

  use_case->selectAndPurchase(domain::SlotId(1));

  EXPECT_EQ(0, use_case->getBalance().getRawValue());
}

// テスト7: 返金機能
TEST_F(PurchaseWithCashUseCaseTest, CanRefund) {
  use_case->startSession();
  use_case->insertCash(domain::Money(300));

  // 返金が呼ばれる
  EXPECT_CALL(mock_coin_mech, dispense(domain::Money(300))).Times(1);

  use_case->refund();

  // 残高が0になっている
  EXPECT_EQ(0, use_case->getBalance().getRawValue());
}

// テスト8: 残高0の場合、返金しても何も起きない
TEST_F(PurchaseWithCashUseCaseTest, RefundWithZeroBalance) {
  use_case->startSession();

  // コインメックは呼ばれない
  EXPECT_CALL(mock_coin_mech, dispense).Times(0);

  EXPECT_NO_THROW(use_case->refund());
}

// テスト9: 複数回の現金投入
TEST_F(PurchaseWithCashUseCaseTest, MultipleInsertions) {
  use_case->startSession();
  use_case->insertCash(domain::Money(100));
  use_case->insertCash(domain::Money(50));
  use_case->insertCash(domain::Money(100));

  EXPECT_EQ(250, use_case->getBalance().getRawValue());
}

// テスト10: 購入後、再度購入可能
TEST_F(PurchaseWithCashUseCaseTest, CanPurchaseMultipleTimes) {
  use_case->startSession();
  use_case->insertCash(domain::Money(500));

  EXPECT_CALL(mock_dispenser, dispense).Times(2);
  EXPECT_CALL(mock_coin_mech, dispense(domain::Money(380))).Times(1);
  EXPECT_CALL(mock_coin_mech, dispense(domain::Money(230))).Times(1);

  // 1回目: スロット1（120円）
  use_case->selectAndPurchase(domain::SlotId(1));
  // 2回目: 新しいセッション開始して再購入
  use_case->startSession();
  use_case->insertCash(domain::Money(380)); // 残りの金額

  use_case->selectAndPurchase(domain::SlotId(2)); // スロット2（150円）

  EXPECT_EQ(0, use_case->getBalance().getRawValue());
}

} // namespace application
} // namespace vending_machine
