/**
 * @file WalletTest.cpp
 * @brief Wallet Aggregate のユニットテスト
 *
 * テスト方針:
 * - 現金投入による残高更新
 * - 電子決済の承認
 * - 出金（支払い）
 * - 返金（お釣り）
 */

#include "domain/payment/Wallet.hpp"
#include "domain/common/Money.hpp"
#include <gtest/gtest.h>
#include <stdexcept>

namespace vending_machine {
namespace domain {
namespace test {

class WalletTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

// ========================================
// 正常系テスト: 生成と初期状態
// ========================================

/**
 * @test 空のWalletを生成できる
 */
TEST_F(WalletTest, CanCreateEmptyWallet) {
  Wallet wallet;

  EXPECT_EQ(0, wallet.getBalance().getRawValue());
  EXPECT_TRUE(wallet.getBalance().isZero());
}

// ========================================
// 現金投入テスト
// ========================================

/**
 * @test 現金を投入できる
 */
TEST_F(WalletTest, CanDepositCash) {
  Wallet wallet;

  wallet.depositCash(Money(100));

  EXPECT_EQ(100, wallet.getBalance().getRawValue());
}

/**
 * @test 複数回の現金投入ができる
 */
TEST_F(WalletTest, CanDepositMultipleTimes) {
  Wallet wallet;

  wallet.depositCash(Money(100));
  wallet.depositCash(Money(50));
  wallet.depositCash(Money(50));

  EXPECT_EQ(200, wallet.getBalance().getRawValue());
}

/**
 * @test 0円を投入してもbalanceは変わらない
 */
TEST_F(WalletTest, DepositingZeroCashDoesNotChange) {
  Wallet wallet;

  wallet.depositCash(Money(100));
  wallet.depositCash(Money(0));

  EXPECT_EQ(100, wallet.getBalance().getRawValue());
}

/**
 * @test 大きな金額を投入できる
 */
TEST_F(WalletTest, CanDepositLargeAmount) {
  Wallet wallet;

  wallet.depositCash(Money(10000));

  EXPECT_EQ(10000, wallet.getBalance().getRawValue());
}

// ========================================
// 出金テスト
// ========================================

/**
 * @test 残高から出金できる
 */
TEST_F(WalletTest, CanWithdraw) {
  Wallet wallet;
  wallet.depositCash(Money(100));

  wallet.withdraw(Money(60));

  EXPECT_EQ(40, wallet.getBalance().getRawValue());
}

/**
 * @test 全額出金できる
 */
TEST_F(WalletTest, CanWithdrawAll) {
  Wallet wallet;
  wallet.depositCash(Money(100));

  wallet.withdraw(Money(100));

  EXPECT_EQ(0, wallet.getBalance().getRawValue());
  EXPECT_TRUE(wallet.getBalance().isZero());
}

/**
 * @test 0円を出金しても変わらない
 */
TEST_F(WalletTest, WithdrawingZeroCashDoesNotChange) {
  Wallet wallet;
  wallet.depositCash(Money(100));

  wallet.withdraw(Money(0));

  EXPECT_EQ(100, wallet.getBalance().getRawValue());
}

/**
 * @test 残高より多く出金しようとすると例外が発生する
 */
TEST_F(WalletTest, ThrowsExceptionWhenWithdrawingMoreThanBalance) {
  Wallet wallet;
  wallet.depositCash(Money(100));

  EXPECT_THROW({ wallet.withdraw(Money(150)); }, std::domain_error);
}

/**
 * @test 0円の残高から出金しようとすると例外が発生する
 */
TEST_F(WalletTest, ThrowsExceptionWhenWithdrawingFromEmpty) {
  Wallet wallet;

  EXPECT_THROW({ wallet.withdraw(Money(1)); }, std::domain_error);
}

/**
 * @test 複数回の出金ができる
 */
TEST_F(WalletTest, CanWithdrawMultipleTimes) {
  Wallet wallet;
  wallet.depositCash(Money(300));

  wallet.withdraw(Money(100));
  wallet.withdraw(Money(50));
  wallet.withdraw(Money(75));

  EXPECT_EQ(75, wallet.getBalance().getRawValue());
}

// ========================================
// 電子決済テスト
// ========================================

/**
 * @test 電子決済を承認できる
 */
TEST_F(WalletTest, CanAuthorizeEMoney) {
  Wallet wallet;

  wallet.authorizeEMoney(Money(500));

  EXPECT_EQ(500, wallet.getBalance().getRawValue());
}

/**
 * @test 現金と電子マネーの残高は合算される
 */
TEST_F(WalletTest, CashAndEMoneyAreAddedTogether) {
  Wallet wallet;

  wallet.depositCash(Money(100));
  wallet.authorizeEMoney(Money(200));

  EXPECT_EQ(300, wallet.getBalance().getRawValue());
}

/**
 * @test 複数回の電子決済承認ができる
 */
TEST_F(WalletTest, CanAuthorizeEMoneyMultipleTimes) {
  Wallet wallet;

  wallet.authorizeEMoney(Money(100));
  wallet.authorizeEMoney(Money(200));
  wallet.authorizeEMoney(Money(150));

  EXPECT_EQ(450, wallet.getBalance().getRawValue());
}

/**
 * @test 0円を承認してもbalanceは変わらない
 */
TEST_F(WalletTest, AuthorizingZeroEMoneyDoesNotChange) {
  Wallet wallet;
  wallet.authorizeEMoney(Money(100));
  wallet.authorizeEMoney(Money(0));

  EXPECT_EQ(100, wallet.getBalance().getRawValue());
}

// ========================================
// 複合操作テスト
// ========================================

/**
 * @test 投入、承認、出金の組み合わせができる
 */
TEST_F(WalletTest, CanPerformCombinedOperations) {
  Wallet wallet;

  wallet.depositCash(Money(100));
  wallet.authorizeEMoney(Money(200));

  EXPECT_EQ(300, wallet.getBalance().getRawValue());

  wallet.withdraw(Money(150));

  EXPECT_EQ(150, wallet.getBalance().getRawValue());
}

/**
 * @test 現金投入後の出金は可能
 */
TEST_F(WalletTest, CanWithdrawAfterDeposit) {
  Wallet wallet;

  wallet.depositCash(Money(200));
  wallet.withdraw(Money(80));
  wallet.depositCash(Money(150));
  wallet.withdraw(Money(100));

  EXPECT_EQ(170, wallet.getBalance().getRawValue());
}

/**
 * @test 電子決済後の出金は可能
 */
TEST_F(WalletTest, CanWithdrawAfterEMoneyAuthorization) {
  Wallet wallet;

  wallet.authorizeEMoney(Money(300));
  wallet.withdraw(Money(120));

  EXPECT_EQ(180, wallet.getBalance().getRawValue());
}

} // namespace test
} // namespace domain
} // namespace vending_machine
