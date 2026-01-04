/**
 * @file SimulatedPaymentGateway.hpp
 * @brief シミュレーション用決済ゲートウェイ
 */

#ifndef VENDING_MACHINE_INFRASTRUCTURE_SIMULATED_PAYMENT_GATEWAY_HPP
#define VENDING_MACHINE_INFRASTRUCTURE_SIMULATED_PAYMENT_GATEWAY_HPP

#include "domain/interfaces/IPaymentGateway.hpp"
#include <iostream>

namespace vending_machine {
namespace infrastructure {

class SimulatedPaymentGateway : public domain::IPaymentGateway {
public:
  void requestPayment(const domain::Price &price) override {
    std::cout << "(決済リクエスト: " << price.getRawValue() << "円)\n";
    status_ = domain::PaymentStatus::AUTHORIZED;
  }

  void cancelPayment() override {
    std::cout << "(決済をキャンセルしました)\n";
    status_ = domain::PaymentStatus::CANCELLED;
  }

  domain::PaymentStatus getPaymentStatus() const override { return status_; }

private:
  domain::PaymentStatus status_ = domain::PaymentStatus::PENDING;
};

} // namespace infrastructure
} // namespace vending_machine

#endif // VENDING_MACHINE_INFRASTRUCTURE_SIMULATED_PAYMENT_GATEWAY_HPP
