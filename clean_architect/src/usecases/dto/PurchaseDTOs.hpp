#ifndef VENDING_MACHINE_USECASES_DTO_PURCHASE_DTOS_HPP
#define VENDING_MACHINE_USECASES_DTO_PURCHASE_DTOS_HPP

#include <string>
#include <vector>

namespace vending_machine {
namespace usecases {
namespace dto {

struct ProductDto {
  int slot_id;
  std::string name;
  int price;
  int stock;
};

struct InsertCashRequest {
  int amount;
};

struct PurchaseRequest {
  int slot_id;
};

struct PurchaseResponse {
  bool success;
  std::string message;
  std::string product_name;
  int change_amount;
};

struct InventoryRefillRequest {
  int slot_id;
  int quantity;
};

struct SalesReportItemDto {
  std::string label; // Slot ID or Payment Method Name
  int count;
  int amount;
};

struct SalesReportDto {
  int total_sales;
  int cash_sales;
  int emoney_sales;
};

struct EMoneyPurchaseRequest {
  int slot_id;
};

struct EMoneyPurchaseResponse {
  bool success;
  std::string message;
  std::string product_name;
};

} // namespace dto
} // namespace usecases
} // namespace vending_machine

#endif // VENDING_MACHINE_USECASES_DTO_PURCHASE_DTOS_HPP
