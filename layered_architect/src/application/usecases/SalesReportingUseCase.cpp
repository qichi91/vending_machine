#include "application/usecases/SalesReportingUseCase.hpp"
#include <map>

namespace vending_machine {
namespace application {

SalesReportingUseCase::SalesReportingUseCase(
    infrastructure::ITransactionHistoryRepository &transaction_history)
    : transaction_history_(transaction_history) {}

std::vector<SlotSalesReport>
SalesReportingUseCase::generateSlotSalesReport() const {
  auto all_records = transaction_history_.getAll();

  // スロット別に集計
  std::map<int, std::pair<int, int>> slot_data; // slot_id -> (count, revenue)

  for (const auto &record : all_records) {
    int slot_id_value = record.getSlotId().getValue();
    int revenue = record.getPrice().getRawValue();

    if (slot_data.find(slot_id_value) == slot_data.end()) {
      slot_data[slot_id_value] = {0, 0};
    }
    slot_data[slot_id_value].first++; // count
    slot_data[slot_id_value].second += revenue;
  }

  // レポート生成
  std::vector<SlotSalesReport> reports;
  for (const auto &entry : slot_data) {
    reports.emplace_back(domain::SlotId(entry.first), entry.second.first,
                         domain::Money(entry.second.second));
  }

  return reports;
}

std::vector<PaymentMethodReport>
SalesReportingUseCase::generatePaymentMethodReport() const {
  auto all_records = transaction_history_.getAll();

  // 決済方法別に集計
  std::map<domain::PaymentMethodType, std::pair<int, int>>
      payment_data; // method -> (count, revenue)

  for (const auto &record : all_records) {
    auto method = record.getPaymentMethod();
    int revenue = record.getPrice().getRawValue();

    if (payment_data.find(method) == payment_data.end()) {
      payment_data[method] = {0, 0};
    }
    payment_data[method].first++; // count
    payment_data[method].second += revenue;
  }

  // レポート生成
  std::vector<PaymentMethodReport> reports;
  for (const auto &entry : payment_data) {
    reports.emplace_back(entry.first, entry.second.first,
                         domain::Money(entry.second.second));
  }

  return reports;
}

domain::Money
SalesReportingUseCase::getRevenueBySlot(const domain::SlotId &slot_id) const {
  auto slot_records = transaction_history_.getBySlotId(slot_id);

  int total = 0;
  for (const auto &record : slot_records) {
    total += record.getPrice().getRawValue();
  }

  return domain::Money(total);
}

int SalesReportingUseCase::getTotalTransactionCount() const {
  return transaction_history_.getAll().size();
}

} // namespace application
} // namespace vending_machine
