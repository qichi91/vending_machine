#include "domain/Product.hpp"
#include "domain/Money.hpp"
#include <gtest/gtest.h>

namespace vending_machine::domain::test {

class ProductTest : public ::testing::Test {};

TEST_F(ProductTest, ShouldCreateProduct) {
  Money price(100);
  Product cola("Cola", price);

  EXPECT_EQ(cola.name(), "Cola");
  EXPECT_EQ(cola.price(), price);
}

TEST_F(ProductTest, ShouldBeEqualIfNameAndPriceAreSame) {
  Money price(100);
  Product p1("Cola", price);
  Product p2("Cola", price);
  Product p3("Water", price);

  EXPECT_EQ(p1, p2);
  EXPECT_NE(p1, p3);
}

} // namespace vending_machine::domain::test