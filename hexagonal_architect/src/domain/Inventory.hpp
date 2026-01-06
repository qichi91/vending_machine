#pragma once

#include "Product.hpp"
#include <map>

namespace vending_machine::domain {

class Inventory {
public:
  void add(const Product &product, int count);
  void reduce(const Product &product);
  bool hasStock(const Product &product) const;
  int getCount(const Product &product) const;

  // 名前から商品を検索する。見つからない場合は std::nullopt
  // を返す設計が望ましいが ここではポインタを返す（見つからない場合は
  // nullptr）。 返されるポインタは Inventory
  // オブジェクトが生きている間のみ有効。
  const Product *findProductByName(const std::string &name) const;

private:
  std::map<Product, int> stocks_;
};

} // namespace vending_machine::domain