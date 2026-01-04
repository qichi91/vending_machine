#include "../../../src/domain/services/PurchaseEligibilityService.hpp"
#include "../../../src/domain/common/Money.hpp"
#include "../../../src/domain/common/Price.hpp"
#include "../../../src/domain/interfaces/ICoinMech.hpp"
#include "../../../src/domain/inventory/Inventory.hpp"
#include "../../../src/domain/inventory/ProductInfo.hpp"
#include "../../../src/domain/inventory/ProductName.hpp"
#include "../../../src/domain/inventory/ProductSlot.hpp"
#include "../../../src/domain/payment/Wallet.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace vending_machine {
namespace domain {

// ICoinMechのモック実装
class MockCoinMech : public ICoinMech {
public:
  MOCK_METHOD(bool, canMakeChange, (const Money &amount), (const override));
  MOCK_METHOD(void, dispense, (const Money &amount), (override));
};

class PurchaseEligibilityServiceTest : public ::testing::Test {
protected:
  void SetUp() override {
    // 在庫セットアップ：複数のスロット
    // スロット1: 価格100、在庫2
    ProductSlot slot1(SlotId(1),
                      ProductInfo(ProductName("Product A"), Price(100)),
                      Quantity(2));
    inventory.addSlot(slot1);

    // スロット2: 価格200、在庫1
    ProductSlot slot2(SlotId(2),
                      ProductInfo(ProductName("Product B"), Price(200)),
                      Quantity(1));
    inventory.addSlot(slot2);

    // スロット3: 価格150、在庫0（在庫なし）
    ProductSlot slot3(SlotId(3),
                      ProductInfo(ProductName("Product C"), Price(150)),
                      Quantity(0));
    inventory.addSlot(slot3);
  }

  Inventory inventory;
  MockCoinMech mock_coin_mech;
};

// テスト1: 残高が十分で、在庫がある商品は適格である
TEST_F(PurchaseEligibilityServiceTest, EligibleWhenSufficientBalanceAndStock) {
  Wallet wallet;
  wallet.depositCash(Money(500));

  // すべての釣銭ケースで釣銭可能と想定
  ON_CALL(mock_coin_mech, canMakeChange).WillByDefault(::testing::Return(true));

  auto eligible = PurchaseEligibilityService::calculateEligibleProducts(
      inventory, wallet, mock_coin_mech);

  // スロット1, 2は購入可能（価格100, 200 < 残高500）
  // スロット3は在庫なしなので不可
  ASSERT_EQ(2, eligible.size());

  // 適格商品のスロットIDを確認
  std::vector<vending_machine::domain::SlotId> eligible_slot_ids;
  for (const auto &product : eligible) {
    eligible_slot_ids.push_back(product.getSlotId());
  }

  EXPECT_TRUE(std::find(eligible_slot_ids.begin(), eligible_slot_ids.end(),
                        SlotId(1)) != eligible_slot_ids.end());
  EXPECT_TRUE(std::find(eligible_slot_ids.begin(), eligible_slot_ids.end(),
                        SlotId(2)) != eligible_slot_ids.end());
}

// テスト2: 残高が不足している場合、適格商品は少なくなる
TEST_F(PurchaseEligibilityServiceTest, IneligibleWhenInsufficientBalance) {
  Wallet wallet;
  wallet.depositCash(Money(150)); // スロット1(100)とスロット3(150)だけ購入可能

  ON_CALL(mock_coin_mech, canMakeChange).WillByDefault(::testing::Return(true));

  auto eligible = PurchaseEligibilityService::calculateEligibleProducts(
      inventory, wallet, mock_coin_mech);

  // スロット1(価格100)のみ購入可能
  // スロット2(価格200 > 残高150)は不可
  // スロット3は在庫なし
  ASSERT_EQ(1, eligible.size());
  EXPECT_EQ(SlotId(1), eligible[0].getSlotId());
}

// テスト3: 在庫がない商品は適格ではない
TEST_F(PurchaseEligibilityServiceTest, IneligibleWhenNoStock) {
  Wallet wallet;
  wallet.depositCash(Money(500)); // 十分な残高

  ON_CALL(mock_coin_mech, canMakeChange).WillByDefault(::testing::Return(true));

  auto eligible = PurchaseEligibilityService::calculateEligibleProducts(
      inventory, wallet, mock_coin_mech);

  // スロット3は在庫0なので適格ではない
  for (const auto &product : eligible) {
    EXPECT_NE(SlotId(3), product.getSlotId());
  }
}

// テスト4: 釣銭準備ができない場合、適格ではない
TEST_F(PurchaseEligibilityServiceTest, IneligibleWhenCannotMakeChange) {
  Wallet wallet;
  wallet.depositCash(Money(250)); // スロット1(100)購入後、釣銭150

  // 釣銭準備ができないと想定
  ON_CALL(mock_coin_mech, canMakeChange)
      .WillByDefault(::testing::Return(false));

  auto eligible = PurchaseEligibilityService::calculateEligibleProducts(
      inventory, wallet, mock_coin_mech);

  // スロット1: 価格100、釣銭150 → 釣銭不可なので適格ではない
  // スロット2: 価格200、釣銭50 → 釣銭不可なので適格ではない
  ASSERT_EQ(0, eligible.size());
}

// テスト5: 釣銭が必要ない場合（残高 == 価格）は適格である
TEST_F(PurchaseEligibilityServiceTest, EligibleWhenExactChangeNoChangeNeeded) {
  Wallet wallet;
  wallet.depositCash(Money(100)); // スロット1の価格と同じ

  // 釣銭が0なので、canMakeChangeは呼ばれない
  EXPECT_CALL(mock_coin_mech, canMakeChange).Times(0);

  auto eligible = PurchaseEligibilityService::calculateEligibleProducts(
      inventory, wallet, mock_coin_mech);

  // スロット1(価格100、釣銭0)は適格
  ASSERT_EQ(1, eligible.size());
  EXPECT_EQ(SlotId(1), eligible[0].getSlotId());
}

// テスト6: 複数スロットでの釣銭判定の混在ケース
TEST_F(PurchaseEligibilityServiceTest, MixedChangeAvailabilityScenario) {
  Wallet wallet;
  wallet.depositCash(Money(250));

  // スロット1(100): 釣銭150 → canMakeChange(150) = true
  // スロット2(200): 釣銭50 → canMakeChange(50) = false
  EXPECT_CALL(mock_coin_mech, canMakeChange(Money(150)))
      .WillOnce(::testing::Return(true));
  EXPECT_CALL(mock_coin_mech, canMakeChange(Money(50)))
      .WillOnce(::testing::Return(false));

  auto eligible = PurchaseEligibilityService::calculateEligibleProducts(
      inventory, wallet, mock_coin_mech);

  // スロット1のみ適格
  ASSERT_EQ(1, eligible.size());
  EXPECT_EQ(SlotId(1), eligible[0].getSlotId());
}

// テスト7: 残高ゼロの場合、何も購入可能ではない
TEST_F(PurchaseEligibilityServiceTest, NoEligibleProductsWithZeroBalance) {
  Wallet wallet;
  // No deposit - balance is 0

  auto eligible = PurchaseEligibilityService::calculateEligibleProducts(
      inventory, wallet, mock_coin_mech);

  ASSERT_EQ(0, eligible.size());
}

// テスト8: 空のインベントリの場合
TEST_F(PurchaseEligibilityServiceTest, EmptyInventoryReturnsEmpty) {
  Inventory empty_inventory;
  Wallet wallet;
  wallet.depositCash(Money(500));

  ON_CALL(mock_coin_mech, canMakeChange).WillByDefault(::testing::Return(true));

  auto eligible = PurchaseEligibilityService::calculateEligibleProducts(
      empty_inventory, wallet, mock_coin_mech);

  ASSERT_EQ(0, eligible.size());
}

// テスト9: 返された適格商品の情報が正確であることを確認
TEST_F(PurchaseEligibilityServiceTest, EligibleProductContainsCorrectInfo) {
  Wallet wallet;
  wallet.depositCash(Money(500));

  ON_CALL(mock_coin_mech, canMakeChange).WillByDefault(::testing::Return(true));

  auto eligible = PurchaseEligibilityService::calculateEligibleProducts(
      inventory, wallet, mock_coin_mech);

  // スロット1の商品情報を確認
  auto slot1_product = std::find_if(
      eligible.begin(), eligible.end(),
      [](const EligibleProduct &p) { return p.getSlotId() == SlotId(1); });

  ASSERT_NE(eligible.end(), slot1_product);
  EXPECT_EQ(ProductName("Product A"),
            slot1_product->getProductInfo().getName());
  EXPECT_EQ(Price(100), slot1_product->getProductInfo().getPrice());
}

// テスト10: 複数スロットでの境界値テスト
TEST_F(PurchaseEligibilityServiceTest, BoundaryValueTest) {
  // 正確に価格と同じ残高
  Wallet wallet;
  wallet.depositCash(Money(100));

  EXPECT_CALL(mock_coin_mech, canMakeChange).Times(0);

  auto eligible = PurchaseEligibilityService::calculateEligibleProducts(
      inventory, wallet, mock_coin_mech);

  // スロット1(価格100、釣銭0)のみ購入可能
  ASSERT_EQ(1, eligible.size());
  EXPECT_EQ(SlotId(1), eligible[0].getSlotId());
}

} // namespace domain
} // namespace vending_machine
