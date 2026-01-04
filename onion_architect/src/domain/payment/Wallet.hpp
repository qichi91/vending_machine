/**
 * @file Wallet.hpp
 * @brief Wallet Aggregate - 通貨管理集約
 *
 * @details
 * Walletは自動販売機の通貨（現金または電子マネー）を管理するアグリゲートです。
 * 投入された現金の計上、または電子決済の承認金額を保持し、出金・返金ロジックを管理します。
 *
 * 注意:
 * 1つの取引では現金または電子マネーのどちらか一方のみを使用します（併用不可）。
 *
 * 責務:
 * - 残高の管理（現金または電子マネー）
 * - 支払い（出金）の実行
 * - 残高の提供
 *
 * @author VendingMachine Team
 * @version 1.0.0
 */

#ifndef VENDING_MACHINE_DOMAIN_PAYMENT_WALLET_HPP
#define VENDING_MACHINE_DOMAIN_PAYMENT_WALLET_HPP

#include "domain/common/Money.hpp"
#include <stdexcept>

namespace vending_machine {
namespace domain {

/**
 * @class Wallet
 * @brief 通貨管理を担当するアグリゲートルート
 *
 * 現金投入または電子決済承認を管理し、統一された残高を提供します。
 * 1つの取引では現金か電子マネーのどちらか一方のみを使用します。
 */
class Wallet {
public:
  /**
   * @brief コンストラクタ
   */
  Wallet();

  /**
   * @brief 現在の残高を取得
   * @return 残高
   */
  Money getBalance() const;

  /**
   * @brief 現金を投入
   * @param amount 投入する金額
   */
  void depositCash(const Money &amount);

  /**
   * @brief 電子決済を承認
   * @param amount 承認する金額
   */
  void authorizeEMoney(const Money &amount);

  /**
   * @brief 支払う（残高から出金）
   * @param amount 支払う金額
   * @throw std::domain_error 残高が支払い額より少ない場合
   */
  void withdraw(const Money &amount);

private:
  Money balance_; ///< 残高（現金または電子マネー）
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_PAYMENT_WALLET_HPP
