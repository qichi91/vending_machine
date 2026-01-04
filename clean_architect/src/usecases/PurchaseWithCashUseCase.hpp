#ifndef VENDING_MACHINE_APPLICATION_USECASES_PURCHASEWITHCASHUSECASE_HPP
#define VENDING_MACHINE_APPLICATION_USECASES_PURCHASEWITHCASHUSECASE_HPP

#include "domain/common/Money.hpp"
#include "domain/inventory/EligibleProduct.hpp"
#include "domain/inventory/SlotId.hpp"
#include "usecases/dto/PurchaseDTOs.hpp"
#include <memory>
#include <vector>

namespace vending_machine {
namespace domain {
class ICoinMech;
class IDispenser;
class ITransactionHistoryRepository;
class Inventory;
class Wallet;
class Sales;
} // namespace domain

namespace usecases {

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
      domain::ITransactionHistoryRepository &transaction_history);

  /**
   * @brief セッションを開始
   */
  void startSession();

  /**
   * @brief 現金を投入
   * @param request 投入金額を含むリクエスト
   */
  void insertCash(const dto::InsertCashRequest &request);

  /**
   * @brief 購入可能な商品一覧を取得
   * @return 購入可能な商品のDTOリスト
   */
  std::vector<dto::ProductDto> getEligibleProducts() const;

  /**
   * @brief 商品を選択して購入
   * @param request 商品スロットIDを含むリクエスト
   * @return 購入結果のレスポンス
   * @throw std::domain_error 商品が購入不可の場合
   */
  dto::PurchaseResponse selectAndPurchase(const dto::PurchaseRequest &request);

  /**
   * @brief 現在の残高を取得
   * @return 残高(int)
   */
  int getBalance() const;

  /**
   * @brief 投入金額を返金（キャンセル）
   * @return 返金額
   */
  int refund();

  /**
   * @brief 全商品一覧を取得（在庫切れ含む）
   * @return 全商品のDTOリスト
   */
  std::vector<dto::ProductDto> getAllProducts() const;

private:
  domain::Inventory &inventory_;
  domain::Wallet &wallet_;
  domain::Sales &sales_;
  domain::ICoinMech &coin_mech_;
  domain::IDispenser &dispenser_;
  domain::ITransactionHistoryRepository &transaction_history_;
};

} // namespace usecases
} // namespace vending_machine

#endif // VENDING_MACHINE_APPLICATION_USECASES_PURCHASEWITHCASHUSECASE_HPP
