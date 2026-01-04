#include "PurchaseWithEMoneyUseCase.hpp"
#include "domain/interfaces/IDispenser.hpp"
#include "domain/interfaces/IPaymentGateway.hpp"
#include "domain/inventory/Inventory.hpp"
#include "domain/payment/Wallet.hpp"
#include "domain/repositories/ITransactionHistoryRepository.hpp"
#include "domain/sales/Sales.hpp"
#include "domain/sales/SessionId.hpp"
#include "domain/sales/TransactionRecord.hpp"
#include <atomic>

namespace vending_machine {
namespace application {

// セッションID生成用カウンター（電子決済用）
static std::atomic<int> emoney_session_counter{1000};

PurchaseWithEMoneyUseCase::PurchaseWithEMoneyUseCase(
    domain::Inventory &inventory, domain::Wallet &wallet, domain::Sales &sales,
    domain::IPaymentGateway &payment_gateway, domain::IDispenser &dispenser,
    domain::ITransactionHistoryRepository &transaction_history)
    : inventory_(inventory), wallet_(wallet), sales_(sales),
      payment_gateway_(payment_gateway), dispenser_(dispenser),
      transaction_history_(transaction_history), pending_price_(std::nullopt) {}

void PurchaseWithEMoneyUseCase::startSession() {
  // 新しいセッションIDを生成
  domain::SessionId session_id(emoney_session_counter++);
  sales_.startSession(session_id);
}

std::vector<domain::EligibleProduct>
PurchaseWithEMoneyUseCase::getAvailableProducts() const {
  std::vector<domain::EligibleProduct> available;

  // 電子決済では在庫があればすべて購入可能
  // （残高チェックは外部決済サーバーが行う）
  const auto &slots = inventory_.getAllSlots();

  for (const auto &pair : slots) {
    const auto &slot_id = pair.first;
    const auto &product_slot = pair.second;

    // 在庫が存在するか確認
    if (product_slot->getStock().getValue() > 0) {
      available.emplace_back(slot_id, product_slot->getProductInfo());
    }
  }

  return available;
}

bool PurchaseWithEMoneyUseCase::selectAndRequestPayment(
    const domain::SlotId &slot_id) {
  // 0. 在庫確認
  const auto &product_slot = inventory_.getSlot(slot_id);
  if (product_slot.getStock().getValue() == 0) {
    throw std::domain_error("Product is out of stock");
  }

  // 1. 商品選択（Sales集約）
  sales_.selectProduct(slot_id);

  // 2. 商品情報取得
  const auto &product_info = product_slot.getProductInfo();
  const auto &price = product_info.getPrice();

  // 3. 決済待ち状態に遷移
  sales_.markPaymentPending();

  // 4. 在庫減算（イベントストーミング Step 5）
  inventory_.dispense(slot_id);

  try {
    // 5. 外部決済ゲートウェイに決済要求（イベントストーミング Step 7準備）
    payment_gateway_.requestPayment(price);

    // 6. 決済ステータス確認
    auto payment_status = payment_gateway_.getPaymentStatus();

    if (payment_status == domain::PaymentStatus::Authorized) {
      // 決済成功時のみ以下を実行

      // 6. 排出中状態に遷移（イベントストーミング Step 6準備）
      sales_.markDispensing();

      // 7. 商品排出（イベントストーミング Step 6）
      const auto &product_info = inventory_.getSlot(slot_id).getProductInfo();
      dispenser_.dispense(product_info);

      // 8. 決済確定（イベントストーミング Step 7）
      // Walletに電子マネー承認額を記録
      domain::Money payment_amount(price.getRawValue());
      wallet_.authorizeEMoney(payment_amount);
      pending_price_ = price;

      // 決済確定（Walletから引き落とし）
      wallet_.withdraw(payment_amount);

      // 9. トランザクション完了
      sales_.completeTransaction();

      // 10. トランザクション履歴を記録
      auto sales_id = sales_.getCurrentSessionSalesId();
      if (sales_id.has_value()) {
        domain::TransactionRecord record(sales_id.value(), slot_id, price,
                                         domain::PaymentMethodType::EMONEY);
        transaction_history_.save(record);
      }

      pending_price_ = std::nullopt;
      return true;

    } else if (payment_status == domain::PaymentStatus::Failed ||
               payment_status == domain::PaymentStatus::Cancelled) {
      // 決済失敗 - 在庫ロールバックとセッションキャンセル
      inventory_.refill(slot_id, domain::Quantity(1));
      sales_.cancelTransaction();
      pending_price_ = std::nullopt;
      return false;

    } else {
      // PENDING状態（実装では即座に結果が返る想定）
      // 実運用では非同期処理が必要
      inventory_.refill(slot_id, domain::Quantity(1));
      sales_.cancelTransaction();
      pending_price_ = std::nullopt;
      return false;
    }
  } catch (const std::exception &e) {
    // 決済確定以降で例外発生 - 在庫ロールバックとセッションキャンセル
    inventory_.refill(slot_id, domain::Quantity(1));
    sales_.cancelTransaction();
    pending_price_ = std::nullopt;
    throw;
  }
}

void PurchaseWithEMoneyUseCase::cancelPayment() {
  if (pending_price_.has_value()) {
    // 決済ゲートウェイに取消要求
    payment_gateway_.cancelPayment();
    pending_price_ = std::nullopt;
  }

  // セッションをキャンセル
  sales_.cancelTransaction();
}

} // namespace application
} // namespace vending_machine
