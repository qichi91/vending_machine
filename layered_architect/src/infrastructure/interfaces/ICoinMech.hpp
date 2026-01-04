#ifndef VENDING_MACHINE_INFRASTRUCTURE_INTERFACES_ICOINMECH_HPP
#define VENDING_MACHINE_INFRASTRUCTURE_INTERFACES_ICOINMECH_HPP

namespace vending_machine {

namespace domain {
class Money;
}

namespace infrastructure {

/**
 * @class ICoinMech
 * @brief コインメック操作のインターフェース
 *
 * 物理的なコインメックの操作を抽象化します。
 * Infrastructure層で定義されます（標準的なレイヤードアーキテクチャ）。
 */
class ICoinMech {
public:
  virtual ~ICoinMech() = default;

  /**
   * @brief 指定した金額のお釣りを準備できるか確認
   * @param amount お釣り金額
   * @return 準備可能な場合true
   */
  virtual bool canMakeChange(const domain::Money &amount) const = 0;

  /**
   * @brief 指定した金額をお釣りとして排出
   * @param amount 排出する金額
   */
  virtual void dispense(const domain::Money &amount) = 0;
};

} // namespace infrastructure
} // namespace vending_machine

#endif // VENDING_MACHINE_INFRASTRUCTURE_INTERFACES_ICOINMECH_HPP
