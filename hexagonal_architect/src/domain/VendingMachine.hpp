#pragma once

#include "Inventory.hpp"
#include "Money.hpp"
#include "Product.hpp"
#include <string>
#include <vector>

namespace vending_machine::domain {

class VendingMachine {
public:
  VendingMachine();

  // 在庫管理用
  void addStock(const Product &product, int count);
  void setInventory(const Inventory &inventory);

  // 操作
  void insertMoney(const Money &money);
  Product purchase(const std::string &productName);
  Product
  purchaseViaElectronicMoney(const std::string &productName); // 電子マネー用
  int refund();

  // 参照
  int currentAmount() const;
  int getSales() const;               // 売上確認
  const Inventory &inventory() const; // For repository saving if needed

private:
  int currentAmount_;
  int totalSales_; // 売上合計
  Inventory inventory_;

  // Helper to find product by name in inventory
  // Since Inventory uses Product as key, we need to iterate or maintain a
  // separate map. Assuming small number of products, iteration is fine.
  const Product *findProductByName(const std::string &name) const;
};

} // namespace vending_machine::domain