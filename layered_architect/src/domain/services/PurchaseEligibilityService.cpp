#include "PurchaseEligibilityService.hpp"
#include "domain/inventory/Inventory.hpp"
#include "domain/payment/Wallet.hpp"
#include "infrastructure/interfaces/ICoinMech.hpp"
#include <algorithm>

namespace vending_machine {
namespace domain {

std::vector<EligibleProduct>
PurchaseEligibilityService::calculateEligibleProducts(
    const Inventory &inventory, const Wallet &wallet,
    const infrastructure::ICoinMech &coin_mech) {
  std::vector<EligibleProduct> eligible;

  // ドメインサービスは複数の集約を横断して処理を行うため、
  // 各集約の詳細にアクセスする必要がある
  const auto &slots = inventory.getAllSlots();

  for (const auto &pair : slots) {
    const auto &slot_id = pair.first;
    const auto &product_slot = pair.second;

    // 在庫が存在するか確認
    if (product_slot->getStock().getValue() == 0) {
      continue;
    }

    const auto &product_info = product_slot->getProductInfo();
    const auto &price = product_info.getPrice();

    // 残高が価格以上か確認
    if (wallet.getBalance().getRawValue() < price.getRawValue()) {
      continue;
    }

    // 釣銭準備が可能か確認
    // 残高が価格を上回る場合、その差分が釣銭として返される
    int balance = wallet.getBalance().getRawValue();
    int price_value = price.getRawValue();
    int change_amount = balance - price_value;

    if (change_amount > 0) {
      Money change(change_amount);
      if (!coin_mech.canMakeChange(change)) {
        continue;
      }
    }

    // すべての条件を満たしているので、購入適格商品として追加
    eligible.emplace_back(slot_id, product_info);
  }

  return eligible;
}

} // namespace domain
} // namespace vending_machine
