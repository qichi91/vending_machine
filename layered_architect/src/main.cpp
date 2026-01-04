#include "application/VendingMachineApplication.hpp"
#include "infrastructure/adapters/SimulatedCoinMech.hpp"
#include "infrastructure/adapters/SimulatedDispenser.hpp"
#include "infrastructure/adapters/SimulatedPaymentGateway.hpp"
#include "infrastructure/repositories/InMemoryTransactionHistoryRepository.hpp"
#include "presentation/ConsoleUI.hpp"
#include <iostream>

int main() {
  try {
    // インフラストラクチャの実装を作成
    vending_machine::infrastructure::InMemoryTransactionHistoryRepository
        transaction_history;
    vending_machine::infrastructure::SimulatedCoinMech coin_mech;
    vending_machine::infrastructure::SimulatedDispenser dispenser;
    vending_machine::infrastructure::SimulatedPaymentGateway payment_gateway;

    // アプリケーションファサードを作成
    vending_machine::application::VendingMachineApplication app(
        coin_mech, dispenser, payment_gateway, transaction_history);

    // 初期在庫を設定
    app.initializeInventory();

    // UIを作成して実行
    vending_machine::presentation::ConsoleUI ui(app);
    ui.run();
  } catch (const std::exception &e) {
    std::cerr << "エラーが発生しました: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
