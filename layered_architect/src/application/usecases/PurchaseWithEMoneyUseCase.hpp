#ifndef VENDING_MACHINE_APPLICATION_USECASES_PURCHASEWITHEMONEYUSECASE_HPP
#define VENDING_MACHINE_APPLICATION_USECASES_PURCHASEWITHEMONEYUSECASE_HPP

#include "domain/common/Money.hpp"
#include "domain/common/Price.hpp"
#include "domain/inventory/EligibleProduct.hpp"
#include "domain/inventory/SlotId.hpp"
#include <memory>
#include <optional>
#include <vector>

namespace vending_machine {
namespace infrastructure {
class IPaymentGateway;
class IDispenser;
class ITransactionHistoryRepository;
} // namespace infrastructure
namespace domain {
class Inventory;
class Wallet;
class Sales;
} // namespace domain

namespace application {

/**
 * @class PurchaseWithEMoneyUseCase
 * @brief 電子決済購入ユースケース
 *
 * QRコード決済を用いた商品購入フローを統括します。
 *
 * 処理フロー：
 * 1. セッション開始
 * 2. 購入可能商品の取得（残高不要、在庫のみチェック）
 * 3. 商品選択
 * 4. 決済要求（IPaymentGateway）
 * 5. 決済承認待ち
 * 6. 決済承認後、Wallet残高更新
 * 7. 在庫減算（Inventory）
 * 8. 決済確定（Wallet）
 * 9. 商品排出（IDispenser）
 * 10. トランザクション完了
 *
 * @note 電子決済では「釣銭準備」の判定は不要（exact payment）
 */
class PurchaseWithEMoneyUseCase {
public:
  /**
   * @brief コンストラクタ
   * @param inventory 在庫集約
   * @param wallet 通貨管理集約
   * @param sales 販売管理集約
   * @param payment_gateway 決済ゲートウェイ
   * @param dispenser ディスペンサー
   * @param transaction_history トランザクション履歴リポジトリ
   */
  PurchaseWithEMoneyUseCase(
      domain::Inventory &inventory, domain::Wallet &wallet,
      domain::Sales &sales, infrastructure::IPaymentGateway &payment_gateway,
      infrastructure::IDispenser &dispenser,
      infrastructure::ITransactionHistoryRepository &transaction_history);

  /**
   * @brief セッションを開始
   */
  void startSession();

  /**
   * @brief 購入可能な商品一覧を取得（在庫あり商品のみ）
   * @return 在庫がある商品のリスト
   *
   * @note 電子決済では残高チェック不要（決済時に外部で確認される）
   */
  std::vector<domain::EligibleProduct> getAvailableProducts() const;

  /**
   * @brief 商品を選択して決済要求
   * @param slot_id 商品スロットID
   * @return 決済が成功した場合true
   * @throw std::domain_error 商品が購入不可の場合
   */
  bool selectAndRequestPayment(const domain::SlotId &slot_id);

  /**
   * @brief 決済をキャンセル
   */
  void cancelPayment();

private:
  domain::Inventory &inventory_;
  domain::Wallet &wallet_;
  domain::Sales &sales_;
  infrastructure::IPaymentGateway &payment_gateway_;
  infrastructure::IDispenser &dispenser_;
  infrastructure::ITransactionHistoryRepository &transaction_history_;

  std::optional<domain::Price> pending_price_; ///< 決済待ち価格
};

} // namespace application
} // namespace vending_machine

#endif // VENDING_MACHINE_APPLICATION_USECASES_PURCHASEWITHEMONEYUSECASE_HPP
