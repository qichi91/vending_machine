/**
 * @file ConsoleUI.hpp
 * @brief ConsoleUI - コンソールベースのユーザーインターフェース
 */

#ifndef VENDING_MACHINE_FRAMEWORKS_DRIVERS_UI_CONSOLEUI_HPP
#define VENDING_MACHINE_FRAMEWORKS_DRIVERS_UI_CONSOLEUI_HPP

#include "interface_adapters/controllers/VendingMachineController.hpp"

namespace vending_machine {
namespace frameworks_drivers {
namespace ui {

/**
 * @class ConsoleUI
 * @brief コンソールベースのユーザーインターフェース
 */
class ConsoleUI {
public:
  /**
   * @brief コンストラクタ
   * @param controller コントローラー
   */
  explicit ConsoleUI(interface_adapters::VendingMachineController &controller);

  /**
   * @brief UIを起動
   */
  void run();

private:
  interface_adapters::VendingMachineController &controller_;

  // UIメソッド
  void showMainMenu();
  void showProductList();
  void handleCashPurchase();
  void handleEMoneyPurchase();
  void showAdminMenu();
  void handleInventoryRefill();
  void handleCashCollection();
  void handleSalesReport();
  void clearScreen();
  void waitForEnter();
};

} // namespace ui
} // namespace frameworks_drivers
} // namespace vending_machine

#endif // VENDING_MACHINE_FRAMEWORKS_DRIVERS_UI_CONSOLEUI_HPP
