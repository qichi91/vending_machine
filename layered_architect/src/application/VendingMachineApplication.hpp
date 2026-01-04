/**
 * @file VendingMachineApplication.hpp
 * @brief VendingMachineApplication - アプリケーションファサード
 *
 * @details
 * 自動販売機システムの全体を統括し、ドメイン・ユースケース・インフラの
 * 依存関係を管理するファサードクラスです。
 *
 * @author VendingMachine Team
 * @version 1.0.0
 */

#ifndef VENDING_MACHINE_APPLICATION_VENDING_MACHINE_APPLICATION_HPP
#define VENDING_MACHINE_APPLICATION_VENDING_MACHINE_APPLICATION_HPP

#include "application/usecases/CashCollectionUseCase.hpp"
#include "application/usecases/InventoryRefillUseCase.hpp"
#include "application/usecases/PurchaseWithCashUseCase.hpp"
#include "application/usecases/PurchaseWithEMoneyUseCase.hpp"
#include "application/usecases/SalesReportingUseCase.hpp"
#include "domain/interfaces/ICoinMech.hpp"
#include "domain/interfaces/IDispenser.hpp"
#include "domain/interfaces/IPaymentGateway.hpp"
#include "domain/inventory/Inventory.hpp"
#include "domain/payment/Wallet.hpp"
#include "domain/sales/Sales.hpp"
#include "infrastructure/repositories/ITransactionHistoryRepository.hpp"
#include <memory>

namespace vending_machine {
namespace application {

/**
 * @class VendingMachineApplication
 * @brief アプリケーション全体を管理するファサード
 *
 * ドメインオブジェクト、ユースケース、インフラの依存関係を
 * 一元管理し、外部（UIなど）にユースケースへのアクセスを提供します。
 */
class VendingMachineApplication {
public:
  /**
   * @brief コンストラクタ
   * @param coin_mech 硬貨処理機
   * @param dispenser ディスペンサー
   * @param payment_gateway 決済ゲートウェイ
   * @param transaction_history 取引履歴リポジトリ
   */
  VendingMachineApplication(
      domain::ICoinMech &coin_mech, domain::IDispenser &dispenser,
      domain::IPaymentGateway &payment_gateway,
      infrastructure::ITransactionHistoryRepository &transaction_history);

  /**
   * @brief 初期在庫を設定
   */
  void initializeInventory();

  // ユースケースへのアクセサ
  PurchaseWithCashUseCase &getPurchaseWithCashUseCase() {
    return *purchase_with_cash_usecase_;
  }

  PurchaseWithEMoneyUseCase &getPurchaseWithEMoneyUseCase() {
    return *purchase_with_emoney_usecase_;
  }

  InventoryRefillUseCase &getInventoryRefillUseCase() {
    return *inventory_refill_usecase_;
  }

  CashCollectionUseCase &getCashCollectionUseCase() {
    return *cash_collection_usecase_;
  }

  SalesReportingUseCase &getSalesReportingUseCase() {
    return *sales_reporting_usecase_;
  }

  // ドメインオブジェクトへのアクセサ（読み取り専用）
  const domain::Inventory &getInventory() const { return inventory_; }
  const domain::Wallet &getWallet() const { return wallet_; }
  const domain::Sales &getSales() const { return sales_; }

private:
  // ドメインオブジェクト
  domain::Inventory inventory_;
  domain::Wallet wallet_;
  domain::Sales sales_;

  // 外部インターフェース（参照で保持）
  domain::ICoinMech &coin_mech_;
  domain::IDispenser &dispenser_;
  domain::IPaymentGateway &payment_gateway_;
  infrastructure::ITransactionHistoryRepository &transaction_history_;

  // ユースケース
  std::unique_ptr<PurchaseWithCashUseCase> purchase_with_cash_usecase_;
  std::unique_ptr<PurchaseWithEMoneyUseCase> purchase_with_emoney_usecase_;
  std::unique_ptr<InventoryRefillUseCase> inventory_refill_usecase_;
  std::unique_ptr<CashCollectionUseCase> cash_collection_usecase_;
  std::unique_ptr<SalesReportingUseCase> sales_reporting_usecase_;
};

} // namespace application
} // namespace vending_machine

#endif // VENDING_MACHINE_APPLICATION_VENDING_MACHINE_APPLICATION_HPP
