#ifndef VENDING_MACHINE_DOMAIN_INTERFACES_ICOINMECH_HPP
#define VENDING_MACHINE_DOMAIN_INTERFACES_ICOINMECH_HPP

namespace vending_machine {
namespace domain {

class Money;

/**
 * @class ICoinMech
 * @brief コインメック操作のインターフェース
 *
 * 物理的なコインメックの操作を抽象化します。
 * DIP（依存性逆転の原則）に基づいて、ドメインロジックは
 * 具体的な実装に依存せずこのインターフェースを使用します。
 */
class ICoinMech {
public:
  virtual ~ICoinMech() = default;

  /**
   * @brief 指定した金額のお釣りを準備できるか確認
   * @param amount お釣り金額
   * @return 準備可能な場合true
   */
  virtual bool canMakeChange(const Money &amount) const = 0;

  /**
   * @brief 指定した金額をお釣りとして排出
   * @param amount 排出する金額
   */
  virtual void dispense(const Money &amount) = 0;
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_INTERFACES_ICOINMECH_HPP
