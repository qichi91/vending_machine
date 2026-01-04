#include "ConsoleUI.hpp"
#include <iostream>
#include <limits>

namespace vending_machine {
namespace frameworks_drivers {
namespace ui {

ConsoleUI::ConsoleUI(interface_adapters::VendingMachineController &controller)
    : controller_(controller) {}

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
  std::cout << "3. 電子決済で購入 (未実装)\n";
  std::cout << "9. 管理者メニュー (未実装)\n";
  std::cout << "0. 終了\n";
  std::cout << "========================================\n";
  std::cout << "選択してください: ";
}

void ConsoleUI::showProductList() {
  clearScreen();
  std::cout << "========================================\n";
  std::cout << "          商品一覧\n";
  std::cout << "========================================\n";

  auto products = controller_.getAllProducts();
  for (const auto &product : products) {
    std::cout << "スロット " << product.slot_id << ": " << product.name << " - "
              << product.price << "円 (在庫: " << product.stock << "個)\n";
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
    controller_.startCashPurchaseSession();

    // Note: In a real app, we might want to get product details here again via
    // controller but for now we proceed.

    // 硬貨投入
    std::cout << "硬貨を投入してください (10, 50, 100, 500円)\n";
    std::cout << "投入完了は 0 を入力\n";

    while (true) {
      std::cout << "現在の投入金額: " << controller_.getBalance() << "円\n";
      std::cout << "硬貨: ";

      int coin;
      std::cin >> coin;

      if (coin == 0) {
        break;
      }

      try {
        controller_.insertCash(coin);
        std::cout << coin << "円を投入しました。\n";
      } catch (const std::exception &e) {
        std::cout << "エラー: " << e.what() << "\n";
      }
    }

    // 購入実行
    try {
      auto response = controller_.purchaseWithCash(slot_num);

      std::cout << "\n購入が完了しました！\n";
      std::cout << "商品: " << response.product_name
                << " をお受け取りください。\n";
      if (response.change_amount > 0) {
        std::cout << "お釣り: " << response.change_amount << "円\n";
      }
    } catch (const std::exception &e) {
      std::cout << "購入エラー: " << e.what() << "\n";
      std::cout << "返金処理を実行します...\n";
      int refunded = controller_.refund();
      std::cout << refunded << "円を返金しました。\n";
    }

  } catch (const std::exception &e) {
    std::cout << "エラー: " << e.what() << "\n";
  }

  waitForEnter();
}

void ConsoleUI::handleEMoneyPurchase() {
  clearScreen();
  std::cout << "========================================\n";
  std::cout << "        電子決済での購入\n";
  std::cout << "========================================\n";

  // 電子決済で購入可能な商品一覧を表示
  auto products = controller_.getAvailableProductsForEMoney();
  for (const auto &product : products) {
    std::cout << "スロット " << product.slot_id << ": " << product.name << " - "
              << product.price << "円 (在庫: " << product.stock << "個)\n";
  }
  std::cout << "========================================\n";

  std::cout
      << "\n購入する商品のスロット番号を入力してください (0:キャンセル): ";
  int slot_num;
  std::cin >> slot_num;

  if (slot_num == 0) {
    return;
  }

  try {
    // セッション開始
    controller_.startEMoneyPurchaseSession();

    // 購入実行
    auto response = controller_.purchaseWithEMoney(slot_num);

    if (response.success) {
      std::cout << "\n決済が完了しました！\n";
      std::cout << "商品: " << response.product_name
                << " をお受け取りください。\n";
    } else {
      std::cout << "\n決済に失敗しました。\n";
      std::cout << "理由: " << response.message << "\n";
    }

  } catch (const std::exception &e) {
    std::cout << "エラー: " << e.what() << "\n";
  }

  waitForEnter();
}

void ConsoleUI::showAdminMenu() {
  bool admin_running = true;
  while (admin_running) {
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "        管理者メニュー\n";
    std::cout << "========================================\n";
    std::cout << "1. 在庫補充\n";
    std::cout << "2. 売上金回収\n";
    std::cout << "3. 売上レポート\n";
    std::cout << "0. 戻る\n";
    std::cout << "========================================\n";
    std::cout << "選択してください: ";

    int choice;
    std::cin >> choice;

    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
      admin_running = false;
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
  std::cout << "--- 在庫補充 ---\n";

  // 現在の在庫を表示
  showProductList();

  int slot_id;
  int quantity;

  std::cout << "\n補充するスロット番号を入力してください: ";
  std::cin >> slot_id;
  std::cout << "補充する数量を入力してください: ";
  std::cin >> quantity;

  try {
    controller_.refillInventory(slot_id, quantity);
    std::cout << "\n補充が完了しました。\n";
  } catch (const std::exception &e) {
    std::cout << "\nエラー: " << e.what() << "\n";
  }
  waitForEnter();
}

void ConsoleUI::handleCashCollection() {
  clearScreen();
  std::cout << "--- 売上金回収 ---\n";

  try {
    int collected = controller_.collectCash();
    std::cout << "\n売上金を回収しました。\n";
    std::cout << "回収金額: " << collected << "円\n";
  } catch (const std::exception &e) {
    std::cout << "\nエラー: " << e.what() << "\n";
  }
  waitForEnter();
}

void ConsoleUI::handleSalesReport() {
  clearScreen();
  std::cout << "--- 売上レポート ---\n";

  try {
    auto report = controller_.getSalesReport();
    std::cout << "総売上: " << report.total_sales << "円\n";
    std::cout << "  - 現金売上: " << report.cash_sales << "円\n";
    std::cout << "  - 電子マネー売上: " << report.emoney_sales << "円\n";
  } catch (const std::exception &e) {
    std::cout << "\nエラー: " << e.what() << "\n";
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

} // namespace ui
} // namespace frameworks_drivers
} // namespace vending_machine
