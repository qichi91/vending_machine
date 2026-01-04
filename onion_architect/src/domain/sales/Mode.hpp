#ifndef VENDING_MACHINE_DOMAIN_SALES_MODE_HPP
#define VENDING_MACHINE_DOMAIN_SALES_MODE_HPP

namespace vending_machine {
namespace domain {

/**
 * @enum Mode
 * @brief システムの動作モード
 */
enum class Mode {
  NORMAL,     ///< 通常モード（購入可能）
  MAINTENANCE ///< メンテナンスモード（購入不可）
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_SALES_MODE_HPP
