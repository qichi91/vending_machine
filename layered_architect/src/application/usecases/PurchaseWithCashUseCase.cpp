#include "PurchaseWithCashUseCase.hpp"
#include "application/repositories/ITransactionHistoryRepository.hpp"
#include "domain/interfaces/ICoinMech.hpp"
#include "domain/interfaces/IDispenser.hpp"
#include "domain/inventory/Inventory.hpp"
#include "domain/payment/Wallet.hpp"
#include "domain/sales/Sales.hpp"
#include "domain/sales/SessionId.hpp"
#include "domain/sales/TransactionRecord.hpp"
#include "domain/services/PurchaseEligibilityService.hpp"
#include <atomic>

namespace vending_machine {
namespace application {

// セッションID生成用カウンター
static std::atomic<int> session_counter{1};

PurchaseWithCashUseCase::PurchaseWithCashUseCase(
    domain::Inventory &inventory, domain::Wallet &wallet, domain::Sales &sales,
    domain::ICoinMech &coin_mech, domain::IDispenser &dispenser,
    application::ITransactionHistoryRepository &transaction_history)
    : inventory_(inventory), wallet_(wallet), sales_(sales),
      coin_mech_(coin_mech), dispenser_(dispenser),
      transaction_history_(transaction_history) {}

void PurchaseWithCashUseCase::startSession() {
  // 新しいセッションIDを生成
  domain::SessionId session_id(session_counter++);
  sales_.startSession(session_id);
}

void PurchaseWithCashUseCase::insertCash(const domain::Money &amount) {
  // Walletに現金を投入
  wallet_.depositCash(amount);
}

std::vector<domain::EligibleProduct>
PurchaseWithCashUseCase::getEligibleProducts() const {
  // ドメインサービスを使用して購入可能商品を算出
  return domain::PurchaseEligibilityService::calculateEligibleProducts(
      inventory_, wallet_, coin_mech_);
}

void PurchaseWithCashUseCase::selectAndPurchase(const domain::SlotId &slot_id) {
  // 1. 商品選択（Sales集約）
  sales_.selectProduct(slot_id);

  // 2. 商品情報取得
  const auto &product_slot = inventory_.getSlot(slot_id);
  const auto &product_info = product_slot.getProductInfo();
  const auto &price = product_info.getPrice();

  // 3. 決済待ち状態に遷移
  sales_.markPaymentPending();

  // 4. 在庫減算
  inventory_.dispense(slot_id);

  // 5. 決済確定（Walletから引き落とし）
  domain::Money payment(price.getRawValue());
  wallet_.withdraw(payment);

  // 6. 排出中状態に遷移
  sales_.markDispensing();

  // 7. 商品排出
  dispenser_.dispense(slot_id);

  // 8. お釣り返却（残高がある場合）
  domain::Money remaining_balance = wallet_.getBalance();
  if (remaining_balance.getRawValue() > 0) {
    coin_mech_.dispense(remaining_balance);
    // 残高をゼロに
    wallet_.withdraw(remaining_balance);
  }

  // 9. トランザクション完了
  sales_.completeTransaction();

  // 10. トランザクション履歴を記録
  auto sales_id = sales_.getCurrentSessionSalesId();
  if (sales_id.has_value()) {
    domain::TransactionRecord record(sales_id.value(), slot_id, price,
                                     domain::PaymentMethodType::CASH);
    transaction_history_.save(record);
  }
}

domain::Money PurchaseWithCashUseCase::getBalance() const {
  return wallet_.getBalance();
}

void PurchaseWithCashUseCase::refund() {
  // 現在の残高を取得
  domain::Money balance = wallet_.getBalance();

  if (balance.getRawValue() > 0) {
    // コインメックで返金
    coin_mech_.dispense(balance);

    // Walletから引き落とし
    wallet_.withdraw(balance);
  }

  // セッションをキャンセル
  sales_.cancelTransaction();
}

} // namespace application
} // namespace vending_machine
