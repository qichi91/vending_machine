/**
 * @file Wallet.cpp
 * @brief Wallet Aggregate の実装
 */

#include "Wallet.hpp"

namespace vending_machine {
namespace domain {

Wallet::Wallet() : balance_(0) {}

Money Wallet::getBalance() const { return balance_; }

void Wallet::depositCash(const Money &amount) { balance_ = balance_ + amount; }

void Wallet::authorizeEMoney(const Money &amount) {
  balance_ = balance_ + amount;
}

void Wallet::withdraw(const Money &amount) {
  if (balance_ < amount) {
    throw std::domain_error("Insufficient balance for withdrawal");
  }

  balance_ = balance_ - amount;
}

} // namespace domain
} // namespace vending_machine
