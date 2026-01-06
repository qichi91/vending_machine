#pragma once

#include "Money.hpp"
#include <string>

namespace vending_machine::domain {

class Product {
public:
  Product(const std::string &name, const Money &price);

  std::string name() const;
  Money price() const;

  bool operator==(const Product &other) const;
  bool operator!=(const Product &other) const;
  bool operator<(const Product &other) const; // for std::map key

private:
  std::string name_;
  Money price_;
};

} // namespace vending_machine::domain