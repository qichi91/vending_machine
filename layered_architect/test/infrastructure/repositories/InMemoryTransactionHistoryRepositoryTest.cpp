#include "infrastructure/repositories/InMemoryTransactionHistoryRepository.hpp"
#include "domain/common/Money.hpp"
#include "domain/common/Price.hpp"
#include "domain/inventory/SlotId.hpp"
#include "domain/sales/SalesId.hpp"
#include "domain/sales/TransactionRecord.hpp"
#include <chrono>
#include <gtest/gtest.h>
#include <thread>

namespace vending_machine {
namespace infrastructure {

class InMemoryTransactionHistoryRepositoryTest : public ::testing::Test {
protected:
  InMemoryTransactionHistoryRepository repository_;

  domain::SlotId slot1_{1};
  domain::SlotId slot2_{2};
  domain::SalesId sales1_{100};
  domain::SalesId sales2_{101};
  domain::Price price1_{150}; // 150円
  domain::Price price2_{100}; // 100円
};

TEST_F(InMemoryTransactionHistoryRepositoryTest, SaveAndRetrieve) {
  auto record1 = domain::TransactionRecord(sales1_, slot1_, price1_,
                                           domain::PaymentMethodType::CASH);
  repository_.save(record1);

  auto all_records = repository_.getAll();
  EXPECT_EQ(1, all_records.size());
  EXPECT_EQ(sales1_, all_records[0].getSalesId());
  EXPECT_EQ(slot1_, all_records[0].getSlotId());
}

TEST_F(InMemoryTransactionHistoryRepositoryTest, SaveMultipleRecords) {
  auto record1 = domain::TransactionRecord(sales1_, slot1_, price1_,
                                           domain::PaymentMethodType::CASH);
  auto record2 = domain::TransactionRecord(sales2_, slot2_, price2_,
                                           domain::PaymentMethodType::EMONEY);

  repository_.save(record1);
  repository_.save(record2);

  auto all_records = repository_.getAll();
  EXPECT_EQ(2, all_records.size());
}

TEST_F(InMemoryTransactionHistoryRepositoryTest, GetBySlotId) {
  auto record1 = domain::TransactionRecord(sales1_, slot1_, price1_,
                                           domain::PaymentMethodType::CASH);
  auto record2 = domain::TransactionRecord(sales2_, slot1_, price2_,
                                           domain::PaymentMethodType::EMONEY);
  auto record3 = domain::TransactionRecord(
      domain::SalesId(102), slot2_, price1_, domain::PaymentMethodType::CASH);

  repository_.save(record1);
  repository_.save(record2);
  repository_.save(record3);

  auto slot1_records = repository_.getBySlotId(slot1_);
  EXPECT_EQ(2, slot1_records.size());
  EXPECT_EQ(slot1_, slot1_records[0].getSlotId());
  EXPECT_EQ(slot1_, slot1_records[1].getSlotId());

  auto slot2_records = repository_.getBySlotId(slot2_);
  EXPECT_EQ(1, slot2_records.size());
  EXPECT_EQ(slot2_, slot2_records[0].getSlotId());
}

TEST_F(InMemoryTransactionHistoryRepositoryTest, GetTotalRevenue) {
  auto record1 = domain::TransactionRecord(
      sales1_, slot1_, price1_, domain::PaymentMethodType::CASH); // 150円
  auto record2 = domain::TransactionRecord(
      sales2_, slot2_, price2_, domain::PaymentMethodType::EMONEY); // 100円

  repository_.save(record1);
  repository_.save(record2);

  domain::Money total_revenue = repository_.getTotalRevenue();
  EXPECT_EQ(250, total_revenue.getRawValue()); // 150 + 100
}

TEST_F(InMemoryTransactionHistoryRepositoryTest,
       GetAllReturnsSortedByTimestamp) {
  // 複数のレコードを時間差をつけて保存
  auto record1 = domain::TransactionRecord(sales1_, slot1_, price1_,
                                           domain::PaymentMethodType::CASH);
  repository_.save(record1);

  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  auto record2 = domain::TransactionRecord(sales2_, slot2_, price2_,
                                           domain::PaymentMethodType::EMONEY);
  repository_.save(record2);

  auto all_records = repository_.getAll();
  EXPECT_EQ(2, all_records.size());
  // 最新のレコードが最初に来るはず（降順）
  EXPECT_EQ(sales2_, all_records[0].getSalesId());
  EXPECT_EQ(sales1_, all_records[1].getSalesId());
}

TEST_F(InMemoryTransactionHistoryRepositoryTest, Clear) {
  auto record1 = domain::TransactionRecord(sales1_, slot1_, price1_,
                                           domain::PaymentMethodType::CASH);
  repository_.save(record1);
  EXPECT_EQ(1, repository_.getAll().size());

  repository_.clear();
  EXPECT_EQ(0, repository_.getAll().size());
  EXPECT_EQ(0, repository_.getTotalRevenue().getRawValue());
}

TEST_F(InMemoryTransactionHistoryRepositoryTest, ClearAndReuse) {
  auto record1 = domain::TransactionRecord(sales1_, slot1_, price1_,
                                           domain::PaymentMethodType::CASH);
  repository_.save(record1);
  repository_.clear();

  auto record2 = domain::TransactionRecord(sales2_, slot2_, price2_,
                                           domain::PaymentMethodType::EMONEY);
  repository_.save(record2);

  auto all_records = repository_.getAll();
  EXPECT_EQ(1, all_records.size());
  EXPECT_EQ(sales2_, all_records[0].getSalesId());
}

TEST_F(InMemoryTransactionHistoryRepositoryTest,
       GetBySlotIdReturnsSortedByTimestamp) {
  auto record1 = domain::TransactionRecord(sales1_, slot1_, price1_,
                                           domain::PaymentMethodType::CASH);
  repository_.save(record1);

  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  auto record2 = domain::TransactionRecord(sales2_, slot1_, price2_,
                                           domain::PaymentMethodType::EMONEY);
  repository_.save(record2);

  auto slot1_records = repository_.getBySlotId(slot1_);
  EXPECT_EQ(2, slot1_records.size());
  // 最新のレコードが最初に来るはず（降順）
  EXPECT_EQ(sales2_, slot1_records[0].getSalesId());
  EXPECT_EQ(sales1_, slot1_records[1].getSalesId());
}

TEST_F(InMemoryTransactionHistoryRepositoryTest,
       GetTotalRevenueWithMultipleSlots) {
  auto record1 = domain::TransactionRecord(sales1_, slot1_, domain::Price(200),
                                           domain::PaymentMethodType::CASH);
  auto record2 =
      domain::TransactionRecord(domain::SalesId(102), slot1_, domain::Price(50),
                                domain::PaymentMethodType::CASH);
  auto record3 = domain::TransactionRecord(domain::SalesId(103), slot2_,
                                           domain::Price(300),
                                           domain::PaymentMethodType::EMONEY);

  repository_.save(record1);
  repository_.save(record2);
  repository_.save(record3);

  domain::Money total_revenue = repository_.getTotalRevenue();
  EXPECT_EQ(550, total_revenue.getRawValue()); // 200 + 50 + 300
}

TEST_F(InMemoryTransactionHistoryRepositoryTest, EmptyRepository) {
  auto all_records = repository_.getAll();
  EXPECT_EQ(0, all_records.size());

  auto slot_records = repository_.getBySlotId(slot1_);
  EXPECT_EQ(0, slot_records.size());

  EXPECT_EQ(0, repository_.getTotalRevenue().getRawValue());
}

} // namespace infrastructure
} // namespace vending_machine
