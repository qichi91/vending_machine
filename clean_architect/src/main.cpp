#include "frameworks_drivers/ui/ConsoleUI.hpp"
#include "interface_adapters/controllers/VendingMachineController.hpp"
#include "interface_adapters/gateways/adapters/SimulatedCoinMech.hpp"
#include "interface_adapters/gateways/adapters/SimulatedDispenser.hpp"
#include "interface_adapters/gateways/adapters/SimulatedPaymentGateway.hpp"
#include "interface_adapters/gateways/repositories/InMemoryTransactionHistoryRepository.hpp"
#include "usecases/VendingMachineApplication.hpp"
#include <iostream>

int main() {
  try {
    // インフラストラクチャの実装を作成
    vending_machine::interface_adapters::InMemoryTransactionHistoryRepository
        transaction_history;
    vending_machine::interface_adapters::SimulatedCoinMech coin_mech;
    vending_machine::interface_adapters::SimulatedDispenser dispenser;
    vending_machine::interface_adapters::SimulatedPaymentGateway
        payment_gateway;

    // アプリケーションファサードを作成
    vending_machine::usecases::VendingMachineApplication app(
        coin_mech, dispenser, payment_gateway, transaction_history);

    // 初期在庫を設定
    app.initializeInventory();

    // コントローラーを作成 (必要なユースケースのみを注入)
    vending_machine::interface_adapters::VendingMachineController controller(
        app.getPurchaseWithCashUseCase(), app.getPurchaseWithEMoneyUseCase(),
        app.getInventoryRefillUseCase(), app.getSalesReportingUseCase(),
        app.getCashCollectionUseCase());

    // UIを作成して実行
    vending_machine::frameworks_drivers::ui::ConsoleUI ui(controller);
    ui.run();
  } catch (const std::exception &e) {
    std::cerr << "エラーが発生しました: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
