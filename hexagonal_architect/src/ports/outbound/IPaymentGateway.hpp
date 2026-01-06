#pragma once

namespace vending_machine::ports::outbound {

class IPaymentGateway {
public:
  virtual ~IPaymentGateway() = default;

  // 金額を指定して決済を行う
  // 成功すれば true, 失敗すれば false
  virtual bool pay(int amount) = 0;
};

} // namespace vending_machine::ports::outbound