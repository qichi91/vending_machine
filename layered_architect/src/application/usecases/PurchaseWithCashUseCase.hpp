#ifndef VENDING_MACHINE_APPLICATION_USECASES_PURCHASEWITHCASHUSECASE_HPP
#define VENDING_MACHINE_APPLICATION_USECASES_PURCHASEWITHCASHUSECASE_HPP

#include "domain/common/Money.hpp"
#include "domain/inventory/EligibleProduct.hpp"
#include "domain/inventory/SlotId.hpp"
#include <memory>
#include <vector>

namespace vending_machine {
namespace application {
class ITransactionHistoryRepository;
}
namespace domain {
class Inventory;
class Wallet;
class Sales;
class ICoinMech;
class IDispenser;
} // namespace domain

namespace application {

/**
 * @class PurchaseWithCashUseCase
 * @brief 現金購入ユースケース
 *
 * 現金投入から商品排出、お釣り返却までの一連の処理を統括します。
 *
 * 処理フロー：
 * 1. セッション開始
 * 2. 現金投入（Wallet残高更新）
 * 3. 購入可能商品の算出（PurchaseEligibilityService）
 * 4. 商品選択（Sales）
 * 5. 在庫減算（Inventory）
 * 6. 決済確定（Wallet）
 * 7. 商品排出（IDispenser）
 * 8. お釣り返却（ICoinMech）
 */
class PurchaseWithCashUseCase {
public:
  /**
   * @brief コンストラクタ
   * @param inventory 在庫集約
   * @param wallet 通貨管理集約
   * @param sales 販売管理集約
   * @param coin_mech コインメック
   * @param dispenser ディスペンサー
   * @param transaction_history トランザクション履歴リポジトリ
   */
  PurchaseWithCashUseCase(
      domain::Inventory &inventory, domain::Wallet &wallet,
      domain::Sales &sales, domain::ICoinMech &coin_mech,
      domain::IDispenser &dispenser,
      application::ITransactionHistoryRepository &transaction_history);

  /**
   * @brief セッションを開始
   */
  void startSession();

  /**
   * @brief 現金を投入
   * @param amount 投入金額
   */
  void insertCash(const domain::Money &amount);

  /**
   * @brief 購入可能な商品一覧を取得
   * @return 購入可能な商品のリスト
   */
  std::vector<domain::EligibleProduct> getEligibleProducts() const;

  /**
   * @brief 商品を選択して購入
   * @param slot_id 商品スロットID
   * @throw std::domain_error 商品が購入不可の場合
   */
  void selectAndPurchase(const domain::SlotId &slot_id);

  /**
   * @brief 現在の残高を取得
   * @return 残高
   */
  domain::Money getBalance() const;

  /**
   * @brief 投入金額を返金（キャンセル）
   */
  void refund();

private:
  domain::Inventory &inventory_;
  domain::Wallet &wallet_;
  domain::Sales &sales_;
  domain::ICoinMech &coin_mech_;
  domain::IDispenser &dispenser_;
  ITransactionHistoryRepository &transaction_history_;
};

} // namespace application
} // namespace vending_machine

#endif // VENDING_MACHINE_APPLICATION_USECASES_PURCHASEWITHCASHUSECASE_HPP
