#include "InMemoryTransactionHistoryRepository.hpp"
#include <algorithm>
#include <numeric>

namespace vending_machine {
namespace infrastructure {

void InMemoryTransactionHistoryRepository::save(
    const domain::TransactionRecord &record) {
  records_.push_back(record);
}

std::vector<domain::TransactionRecord>
InMemoryTransactionHistoryRepository::getAll() const {
  // タイムスタンプの降順でソート
  auto sorted_records = records_;
  std::sort(sorted_records.begin(), sorted_records.end(),
            [](const domain::TransactionRecord &a,
               const domain::TransactionRecord &b) {
              return a.getTimestamp() > b.getTimestamp();
            });
  return sorted_records;
}

std::vector<domain::TransactionRecord>
InMemoryTransactionHistoryRepository::getBySlotId(
    const domain::SlotId &slot_id) const {
  std::vector<domain::TransactionRecord> result;
  std::copy_if(records_.begin(), records_.end(), std::back_inserter(result),
               [&slot_id](const domain::TransactionRecord &record) {
                 return record.getSlotId() == slot_id;
               });
  // タイムスタンプの降順でソート
  std::sort(result.begin(), result.end(),
            [](const domain::TransactionRecord &a,
               const domain::TransactionRecord &b) {
              return a.getTimestamp() > b.getTimestamp();
            });
  return result;
}

domain::Money InMemoryTransactionHistoryRepository::getTotalRevenue() const {
  int total =
      std::accumulate(records_.begin(), records_.end(), 0,
                      [](int sum, const domain::TransactionRecord &record) {
                        return sum + record.getPrice().getRawValue();
                      });
  return domain::Money(total);
}

void InMemoryTransactionHistoryRepository::clear() { records_.clear(); }

} // namespace infrastructure
} // namespace vending_machine
