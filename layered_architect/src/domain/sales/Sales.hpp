#ifndef VENDING_MACHINE_DOMAIN_SALES_SALES_HPP
#define VENDING_MACHINE_DOMAIN_SALES_SALES_HPP

#include "Mode.hpp"
#include "SalesId.hpp"
#include "TransactionSession.hpp"
#include <memory>
#include <optional>

namespace vending_machine {
namespace domain {

/**
 * @class Sales
 * @brief 販売管理を担当する集約ルート
 *
 * システムモード（通常/メンテナンス）の管理と、
 * 取引セッションのライフサイクルを制御します。
 */
class Sales {
public:
  /**
   * @brief コンストラクタ
   * @param sales_id 販売管理ID
   */
  explicit Sales(const SalesId &sales_id);

  /**
   * @brief 販売管理IDを取得
   * @return 販売管理ID
   */
  const SalesId &getId() const;

  /**
   * @brief 現在のモードを取得
   * @return システムモード
   */
  Mode getMode() const;

  /**
   * @brief 現在のセッションを取得
   * @return セッション（存在しない場合はnullptr）
   */
  const TransactionSession *getCurrentSession() const;

  /**
   * @brief 現在のセッションの販売IDを取得
   * @return 販売ID（セッションが存在しない場合はstd::nullopt）
   */
  std::optional<SalesId> getCurrentSessionSalesId() const;

  /**
   * @brief 新しいセッションを開始
   * @param session_id セッションID
   * @throw std::domain_error メンテナンスモード中の場合
   * @throw std::domain_error 既にセッションが存在する場合
   */
  void startSession(const SessionId &session_id);

  /**
   * @brief 商品を選択
   * @param slot_id 選択するスロットID
   * @throw std::domain_error セッションが存在しない場合
   */
  void selectProduct(const SlotId &slot_id);

  /**
   * @brief 決済待ち状態に遷移
   * @throw std::domain_error セッションが存在しない場合
   */
  void markPaymentPending();

  /**
   * @brief 排出中状態に遷移
   * @throw std::domain_error セッションが存在しない場合
   */
  void markDispensing();

  /**
   * @brief 取引を完了してセッションを終了
   * @throw std::domain_error セッションが存在しない場合
   */
  void completeTransaction();

  /**
   * @brief 取引をキャンセルしてセッションを終了
   * @throw std::domain_error セッションが存在しない場合
   */
  void cancelTransaction();

  /**
   * @brief メンテナンスモードに切り替え
   * @throw std::domain_error 進行中のセッションが存在する場合
   */
  void startMaintenance();

  /**
   * @brief 通常モードに切り替え
   */
  void endMaintenance();

private:
  SalesId sales_id_;                                    ///< 販売管理ID
  Mode mode_;                                           ///< 現在のモード
  std::unique_ptr<TransactionSession> current_session_; ///< 現在のセッション
};

} // namespace domain
} // namespace vending_machine

#endif // VENDING_MACHINE_DOMAIN_SALES_SALES_HPP
