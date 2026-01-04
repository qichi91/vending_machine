/**
 * @file SimulatedPaymentGateway.hpp
 * @brief シミュレーション用決済ゲートウェイ
 */

#ifndef VENDING_MACHINE_INFRASTRUCTURE_SIMULATED_PAYMENT_GATEWAY_HPP
#define VENDING_MACHINE_INFRASTRUCTURE_SIMULATED_PAYMENT_GATEWAY_HPP

#include "infrastructure/interfaces/IPaymentGateway.hpp"
#include <iostream>

namespace vending_machine {
namespace infrastructure {

class SimulatedPaymentGateway : public IPaymentGateway {
public:
  void requestPayment(const domain::Price &price) override {
    std::cout << "(決済リクエスト: " << price.getRawValue() << "円)\n";
    status_ = PaymentStatus::Authorized;
  }

  void cancelPayment() override {
    std::cout << "(決済をキャンセルしました)\n";
    status_ = PaymentStatus::Cancelled;
  }

  PaymentStatus getPaymentStatus() const override { return status_; }

private:
  PaymentStatus status_ = PaymentStatus::Pending;
};

} // namespace infrastructure
} // namespace vending_machine

#endif // VENDING_MACHINE_INFRASTRUCTURE_SIMULATED_PAYMENT_GATEWAY_HPP
