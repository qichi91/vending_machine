#pragma once

#include "domain/Money.hpp"
#include "domain/Product.hpp"
#include <string>
#include <vector>

namespace vending_machine::ports::inbound {

using namespace domain;

class IVendingMachineService {
public:
  virtual ~IVendingMachineService() = default;

  // お金を投入する
  virtual void insertMoney(const Money &money) = 0;

  // 商品を選択し、購入する
  // 購入成功時はメッセージ等を返すか、voidで例外を投げるか。
  // ここではシンプルに購入した商品を返すこととする（取り出し口に出るイメージ）。
  virtual Product selectProduct(const std::string &productName) = 0;

  // 現在の投入金額を取得する
  virtual int getCurrentAmount() const = 0;

  // 払い戻しを実行し、お釣りを受け取る
  // 実際には硬貨のリストなどが返るべきだが、簡略化して合計金額のMoneyを返すか、
  // あるいは Money のリストを返す。
  // ドメインのMoneyは有効貨幣しか持てないので、お釣りとして 150円 を返すには
  // 100円と50円のリストである必要がある。 とりあえず簡略化のため int (金額)
  // を返す仕様にするか？ いや、そこは頑張って std::vector<Money> を返すべき。
  virtual std::vector<Money> getRefund() = 0;

  // 電子マネーで商品を購入する
  virtual Product
  purchaseWithElectronicMoney(const std::string &productName) = 0;
};

} // namespace vending_machine::ports::inbound