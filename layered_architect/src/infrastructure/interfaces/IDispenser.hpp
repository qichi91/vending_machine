#ifndef VENDING_MACHINE_INFRASTRUCTURE_INTERFACES_IDISPENSER_HPP
#define VENDING_MACHINE_INFRASTRUCTURE_INTERFACES_IDISPENSER_HPP

namespace vending_machine {

namespace domain {
class ProductInfo;
}

namespace infrastructure {

/**
 * @class IDispenser
 * @brief ディスペンサー操作のインターフェース
 *
 * 物理的なディスペンサーの操作を抽象化します。
 * Infrastructure層で定義されます（標準的なレイヤードアーキテクチャ）。
 */
class IDispenser {
public:
  virtual ~IDispenser() = default;

  /**
   * @brief 商品を排出できるか確認
   * @param product 商品情報
   * @return 排出可能な場合true
   */
  virtual bool canDispense(const domain::ProductInfo &product) const = 0;

  /**
   * @brief 商品を排出
   * @param product 排出する商品情報
   */
  virtual void dispense(const domain::ProductInfo &product) = 0;
};

} // namespace infrastructure
} // namespace vending_machine

#endif // VENDING_MACHINE_INFRASTRUCTURE_INTERFACES_IDISPENSER_HPP
