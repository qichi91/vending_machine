/**
 * @file ConsoleUI.hpp
 * @brief ConsoleUI - コンソールベースのユーザーインターフェース
 *
 * @details
 * 自動販売機システムをコマンドラインから操作するためのUIを提供します。
 *
 * @author VendingMachine Team
 * @version 1.0.0
 */

#ifndef VENDING_MACHINE_PRESENTATION_CONSOLEUI_HPP
#define VENDING_MACHINE_PRESENTATION_CONSOLEUI_HPP

#include "application/VendingMachineApplication.hpp"
#include <memory>

namespace vending_machine {
namespace presentation {

/**
 * @class ConsoleUI
 * @brief コンソールベースのユーザーインターフェース
 */
class ConsoleUI {
public:
  /**
   * @brief コンストラクタ
   * @param app アプリケーションファサード
   */
  explicit ConsoleUI(application::VendingMachineApplication &app);

  /**
   * @brief UIを起動
   */
  void run();

private:
  application::VendingMachineApplication &app_;

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

} // namespace presentation
} // namespace vending_machine

#endif // VENDING_MACHINE_PRESENTATION_CONSOLEUI_HPP
