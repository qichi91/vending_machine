#pragma once

#include "ports/outbound/IPaymentGateway.hpp"

namespace vending_machine::adapters::outbound {

using namespace ports::outbound;

class MockPaymentGateway : public IPaymentGateway {
public:
  bool pay(int amount) override {
    // 常に成功するダミー実装
    // 本来は外部APIなどを呼ぶ
    return true;
  }
};

} // namespace vending_machine::adapters::outbound