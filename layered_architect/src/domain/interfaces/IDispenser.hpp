#ifndef VENDING_MACHINE_DOMAIN_INTERFACES_IDISPENSER_HPP
#define VENDING_MACHINE_DOMAIN_INTERFACES_IDISPENSER_HPP

namespace vending_machine {
namespace domain {

class SlotId;

/**
 * @class IDispenser
 * @brief 商品排出装置のインターフェース
 *
 * 指定されたスロットから商品を排出する操作を抽象化します。
 */
class IDispenser {
public:
  virtual ~IDispenser() = default;

  /**
   * @brief 指定されたスロットから商品を排出
   * @param slot_id 排出するスロットID
   */
  virtual void dispense(const SlotId &slot_id) = 0;
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_INTERFACES_IDISPENSER_HPP
