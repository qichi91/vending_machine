#include "presentation/ConsoleUI.hpp"
#include <iostream>
#include <limits>

namespace vending_machine {
namespace presentation {

ConsoleUI::ConsoleUI(application::VendingMachineApplication &app) : app_(app) {}

void ConsoleUI::run() {
  bool running = true;

  while (running) {
    showMainMenu();

    int choice;
    std::cin >> choice;

    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "\n無効な入力です。数字を入力してください。\n";
      waitForEnter();
      continue;
    }

    switch (choice) {
    case 1:
      showProductList();
      break;
    case 2:
      handleCashPurchase();
      break;
    case 3:
      handleEMoneyPurchase();
      break;
    case 9:
      showAdminMenu();
      break;
    case 0:
      std::cout << "\nご利用ありがとうございました。\n";
      running = false;
      break;
    default:
      std::cout << "\n無効な選択です。\n";
      waitForEnter();
      break;
    }
  }
}

void ConsoleUI::showMainMenu() {
  clearScreen();
  std::cout << "========================================\n";
  std::cout << "      自動販売機システム\n";
  std::cout << "========================================\n";
  std::cout << "1. 商品一覧\n";
  std::cout << "2. 現金で購入\n";
  std::cout << "3. 電子決済で購入\n";
  std::cout << "9. 管理者メニュー\n";
  std::cout << "0. 終了\n";
  std::cout << "========================================\n";
  std::cout << "選択してください: ";
}

void ConsoleUI::showProductList() {
  clearScreen();
  std::cout << "========================================\n";
  std::cout << "          商品一覧\n";
  std::cout << "========================================\n";

  for (int i = 1; i <= 4; i++) {
    try {
      const auto &slot = app_.getInventory().getSlot(domain::SlotId(i));
      std::cout << "スロット " << i << ": "
                << slot.getProductInfo().getName().getValue() << " - "
                << slot.getProductInfo().getPrice().getRawValue()
                << "円 (在庫: " << slot.getStock().getValue() << "個)\n";
    } catch (...) {
      // スロットが存在しない場合はスキップ
    }
  }

  std::cout << "========================================\n";
  waitForEnter();
}

void ConsoleUI::handleCashPurchase() {
  clearScreen();
  std::cout << "========================================\n";
  std::cout << "        現金での購入\n";
  std::cout << "========================================\n";

  showProductList();

  std::cout
      << "\n購入する商品のスロット番号を入力してください (0:キャンセル): ";
  int slot_num;
  std::cin >> slot_num;

  if (slot_num == 0) {
    return;
  }

  try {
    // セッション開始
    app_.getPurchaseWithCashUseCase().startSession();

    const auto &slot = app_.getInventory().getSlot(domain::SlotId(slot_num));
    int price = slot.getProductInfo().getPrice().getRawValue();

    std::cout << "\n商品: " << slot.getProductInfo().getName().getValue()
              << "\n";
    std::cout << "価格: " << price << "円\n\n";

    // 硬貨投入
    std::cout << "硬貨を投入してください (10, 50, 100, 500円)\n";
    std::cout << "投入完了は 0 を入力\n";

    while (true) {
      std::cout << "現在の投入金額: "
                << app_.getWallet().getBalance().getRawValue() << "円\n";
      std::cout << "硬貨: ";

      int coin;
      std::cin >> coin;

      if (coin == 0) {
        break;
      }

      try {
        app_.getPurchaseWithCashUseCase().insertCash(domain::Money(coin));
        std::cout << coin << "円を投入しました。\n";
      } catch (const std::exception &e) {
        std::cout << "エラー: " << e.what() << "\n";
      }
    }

    // 購入実行
    try {
      app_.getPurchaseWithCashUseCase().selectAndPurchase(
          domain::SlotId(slot_num));

      std::cout << "\n購入が完了しました！\n";
      std::cout << "商品: " << slot.getProductInfo().getName().getValue()
                << " をお受け取りください。\n";
    } catch (const std::exception &e) {
      std::cout << "購入エラー: " << e.what() << "\n";
      std::cout << "返金処理を実行します...\n";
      app_.getPurchaseWithCashUseCase().refund();
    }

  } catch (const std::exception &e) {
    std::cout << "エラー: " << e.what() << "\n";
  }

  waitForEnter();
}

void ConsoleUI::handleEMoneyPurchase() {
  clearScreen();
  std::cout << "========================================\n";
  std::cout << "       電子決済での購入\n";
  std::cout << "========================================\n";

  showProductList();

  std::cout
      << "\n購入する商品のスロット番号を入力してください (0:キャンセル): ";
  int slot_num;
  std::cin >> slot_num;

  if (slot_num == 0) {
    return;
  }

  try {
    // セッション開始
    app_.getPurchaseWithEMoneyUseCase().startSession();

    const auto &slot = app_.getInventory().getSlot(domain::SlotId(slot_num));
    std::cout << "\n商品: " << slot.getProductInfo().getName().getValue()
              << "\n";
    std::cout << "価格: " << slot.getProductInfo().getPrice().getRawValue()
              << "円\n";
    std::cout << "\nQRコードを表示しています...\n";
    std::cout << "スマートフォンで読み取って決済してください。\n";
    std::cout << "\n決済が完了したら Enter キーを押してください...\n";
    waitForEnter();

    // 決済実行
    bool success = app_.getPurchaseWithEMoneyUseCase().selectAndRequestPayment(
        domain::SlotId(slot_num));

    if (success) {
      std::cout << "\n購入が完了しました！\n";
      std::cout << "商品: " << slot.getProductInfo().getName().getValue()
                << " をお受け取りください。\n";
    } else {
      std::cout << "\n決済に失敗しました。\n";
    }

  } catch (const std::exception &e) {
    std::cout << "エラー: " << e.what() << "\n";
  }

  waitForEnter();
}

void ConsoleUI::showAdminMenu() {
  bool in_admin = true;

  while (in_admin) {
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "        管理者メニュー\n";
    std::cout << "========================================\n";
    std::cout << "1. 在庫補充\n";
    std::cout << "2. 売上回収\n";
    std::cout << "3. 売上レポート\n";
    std::cout << "0. メインメニューに戻る\n";
    std::cout << "========================================\n";
    std::cout << "選択してください: ";

    int choice;
    std::cin >> choice;

    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "\n無効な入力です。\n";
      waitForEnter();
      continue;
    }

    switch (choice) {
    case 1:
      handleInventoryRefill();
      break;
    case 2:
      handleCashCollection();
      break;
    case 3:
      handleSalesReport();
      break;
    case 0:
      in_admin = false;
      break;
    default:
      std::cout << "\n無効な選択です。\n";
      waitForEnter();
      break;
    }
  }
}

void ConsoleUI::handleInventoryRefill() {
  clearScreen();
  std::cout << "========================================\n";
  std::cout << "          在庫補充\n";
  std::cout << "========================================\n";

  showProductList();

  std::cout << "\n補充するスロット番号を入力してください (0:キャンセル): ";
  int slot_num;
  std::cin >> slot_num;

  if (slot_num == 0) {
    return;
  }

  std::cout << "補充する個数を入力してください: ";
  int quantity;
  std::cin >> quantity;

  try {
    app_.getInventoryRefillUseCase().refillSlot(domain::SlotId(slot_num),
                                                domain::Quantity(quantity));
    std::cout << "\nスロット " << slot_num << " に " << quantity
              << "個補充しました。\n";
    std::cout << "現在の在庫: "
              << app_.getInventoryRefillUseCase()
                     .getSlotQuantity(domain::SlotId(slot_num))
                     .getValue()
              << "個\n";
  } catch (const std::exception &e) {
    std::cout << "エラー: " << e.what() << "\n";
  }

  waitForEnter();
}

void ConsoleUI::handleCashCollection() {
  clearScreen();
  std::cout << "========================================\n";
  std::cout << "          売上回収\n";
  std::cout << "========================================\n";

  auto current_revenue = app_.getCashCollectionUseCase().getTotalRevenue();
  std::cout << "現在の売上: " << current_revenue.getRawValue() << "円\n\n";

  std::cout << "売上を回収しますか? (1:はい / 0:いいえ): ";
  int confirm;
  std::cin >> confirm;

  if (confirm == 1) {
    auto collected = app_.getCashCollectionUseCase().collectCash();
    std::cout << "\n" << collected.getRawValue() << "円を回収しました。\n";
  }

  waitForEnter();
}

void ConsoleUI::handleSalesReport() {
  clearScreen();
  std::cout << "========================================\n";
  std::cout << "         売上レポート\n";
  std::cout << "========================================\n";

  auto total_transactions =
      app_.getSalesReportingUseCase().getTotalTransactionCount();
  std::cout << "総取引数: " << total_transactions << "件\n\n";

  // スロット別レポート
  std::cout << "【スロット別売上】\n";
  auto slot_reports = app_.getSalesReportingUseCase().generateSlotSalesReport();
  for (const auto &report : slot_reports) {
    const auto &slot = app_.getInventory().getSlot(report.slot_id);
    std::cout << "スロット " << report.slot_id.getValue() << " ("
              << slot.getProductInfo().getName().getValue()
              << "): " << report.transaction_count << "回 / "
              << report.total_revenue.getRawValue() << "円\n";
  }

  std::cout << "\n【決済方法別売上】\n";
  auto payment_reports =
      app_.getSalesReportingUseCase().generatePaymentMethodReport();
  for (const auto &report : payment_reports) {
    std::string method_name =
        (report.payment_method == domain::PaymentMethodType::CASH) ? "現金"
                                                                   : "電子決済";
    std::cout << method_name << ": " << report.transaction_count << "回 / "
              << report.total_revenue.getRawValue() << "円\n";
  }

  waitForEnter();
}

void ConsoleUI::clearScreen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void ConsoleUI::waitForEnter() {
  std::cout << "\nEnter キーを押して続行...";
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::cin.get();
}

} // namespace presentation
} // namespace vending_machine
