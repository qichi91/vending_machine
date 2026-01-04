#ifndef VENDING_MACHINE_DOMAIN_SERVICES_PURCHASEELIGIBILITYSERVICE_HPP
#define VENDING_MACHINE_DOMAIN_SERVICES_PURCHASEELIGIBILITYSERVICE_HPP

#include "domain/inventory/EligibleProduct.hpp"
#include <memory>
#include <vector>

namespace vending_machine {
namespace domain {

class Inventory;
class Wallet;
class ICoinMech;

/**
 * @class PurchaseEligibilityService
 * @brief 購入適格性判定ドメインサービス
 *
 * 複数の集約（Inventory、Wallet）から情報を集約し、
 * 現在の状況で「有効な商品」がどれかを判定します。
 *
 * 判定ロジック：
 * - 在庫 > 0
 * - かつ 残高 >= 価格
 * - かつ 釣銭準備OK（ICoinMechで確認）
 * のすべての条件を満たす商品が購入可能です。
 */
class PurchaseEligibilityService {
public:
  /**
   * @brief 購入可能な商品一覧を取得
   * @param inventory 在庫集約
   * @param wallet 通貨管理集約
   * @param coin_mech コインメック（釣銭準備確認用）
   * @return 購入可能な商品のリスト
   */
  static std::vector<EligibleProduct>
  calculateEligibleProducts(const Inventory &inventory, const Wallet &wallet,
                            const ICoinMech &coin_mech);
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_SERVICES_PURCHASEELIGIBILITYSERVICE_HPP
