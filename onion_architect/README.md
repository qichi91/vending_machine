# Onion Architecture - 自動販売機システム

このディレクトリは、**オニオンアーキテクチャ（Onion Architecture）** で実装された自動販売機システムです。

## 概要

オニオンアーキテクチャは、ドメインモデルを中心に据え、依存関係を外側から内側（ドメイン）に向かわせることで、ビジネスロジックの独立性とテスタビリティを高める設計パターンです。

```
      Infrastructure Layer (インフラ層)
            ↓ (依存)
      Application Layer (アプリ層)
            ↓ (依存)
┌─────────────────────────────────────┐
│   Domain Layer (ドメイン層)          │
│                                     │
│   Entities, Value Objects           │
│   Domain Services                   │
│   Repository Interfaces             │
│   Gateway Interfaces                │
└─────────────────────────────────────┘
```

### 依存関係の流れ

```
Infrastructure → Application → Domain ← Infrastructure (Implements)
```

**各層の責務：**
- **Domain層**: ビジネスロジックの中核。エンティティ、値オブジェクト、ドメインサービス、**インターフェース定義**。
- **Application層**: ユースケース実装。ドメインオブジェクトを調整してタスクを遂行。
- **Infrastructure層**: ドメイン層で定義されたインターフェースの実装（DB、ハードウェア、UI）。
- **Presentation層**: ユーザーインターフェース（ConsoleUI）。

**依存関係の原則（Dependency Inversion Principle）：**
- **すべての依存は内側（Domain）に向かう**。
- Domain層は他のどの層にも依存しない（外部ライブラリやフレームワークへの依存も極力排除）。
- Infrastructure層はDomain層で定義されたインターフェースを実装する（依存性逆転）。

---

## ディレクトリ構成

```
onion_architect/
├── CMakeLists.txt                          # CMake設定ファイル
├── src/
│   ├── main.cpp                             # エントリーポイント（DIの設定）
│   ├── presentation/
│   │   ├── ConsoleUI.hpp                   # UI定義
│   │   └── ConsoleUI.cpp                   # UI実装
│   ├── application/
│   │   ├── VendingMachineApplication.hpp   # Facade
│   │   ├── VendingMachineApplication.cpp
│   │   └── usecases/
│   │       ├── PurchaseWithCashUseCase.hpp
│   │       ├── PurchaseWithCashUseCase.cpp
│   │       ├── PurchaseWithEMoneyUseCase.hpp
│   │       ├── PurchaseWithEMoneyUseCase.cpp
│   │       ├── InventoryRefillUseCase.hpp
│   │       ├── InventoryRefillUseCase.cpp
│   │       ├── CashCollectionUseCase.hpp
│   │       ├── CashCollectionUseCase.cpp
│   │       ├── SalesReportingUseCase.hpp
│   │       └── SalesReportingUseCase.cpp
│   ├── domain/
│   │   ├── common/
│   │   │   ├── Money.hpp                   # 金額値オブジェクト
│   │   │   ├── Price.hpp                   # 商品価格値オブジェクト
│   │   │   └── Quantity.hpp                # 数量値オブジェクト
│   │   ├── inventory/
│   │   │   ├── Inventory.hpp               # 在庫集約
│   │   │   ├── ProductSlot.hpp             # 商品スロットエンティティ
│   │   │   ├── ProductInfo.hpp             # 商品情報値オブジェクト
│   │   │   └── SlotId.hpp                  # スロットID値オブジェクト
│   │   ├── payment/
│   │   │   └── Wallet.hpp                  # 通貨管理集約
│   │   ├── sales/
│   │   │   ├── Sales.hpp                   # 販売管理集約
│   │   │   ├── TransactionRecord.hpp       # 取引記録値オブジェクト
│   │   │   └── SessionId.hpp               # セッションID値オブジェクト
│   │   ├── services/
│   │   │   └── PurchaseEligibilityService.hpp # ドメインサービス
│   │   ├── interfaces/                     # ★ インターフェースはドメイン層に配置
│   │   │   ├── ICoinMech.hpp
│   │   │   ├── IDispenser.hpp
│   │   │   └── IPaymentGateway.hpp
│   │   └── repositories/                   # ★ リポジトリインターフェースもドメイン層
│   │       └── ITransactionHistoryRepository.hpp
│   └── infrastructure/
│       ├── adapters/                       # ★ ドメインインターフェースの実装
│       │   ├── SimulatedCoinMech.hpp
│       │   ├── SimulatedDispenser.hpp
│       │   └── SimulatedPaymentGateway.hpp
│       └── repositories/
│           └── InMemoryTransactionHistoryRepository.hpp
├── test/                                    # テストスイート
│   ├── application/usecases/
│   │   ├── PurchaseWithCashUseCaseTest.cpp
│   │   ├── PurchaseWithEMoneyUseCaseTest.cpp
│   │   ├── InventoryRefillUseCaseTest.cpp
│   │   ├── CashCollectionUseCaseTest.cpp
│   │   └── SalesReportingUseCaseTest.cpp
│   ├── domain/
│   │   ├── common/
│   │   ├── inventory/
│   │   ├── payment/
│   │   ├── sales/
│   │   └── services/
│   └── infrastructure/
│       └── repositories/
└── build/                                   # ビルド出力ディレクトリ
```

---

## アーキテクチャの特徴

### 1. 依存性逆転原則（DIP）の適用

Domain層がインターフェースを定義し、Infrastructure層が実装：

```
Domain Layer (Interfaces):
  ├── domain/interfaces/ICoinMech.hpp
  ├── domain/interfaces/IDispenser.hpp
  └── domain/interfaces/IPaymentGateway.hpp

Infrastructure Layer (Implementation):
  ├── infrastructure/adapters/SimulatedCoinMech.cpp
  ├── infrastructure/adapters/SimulatedDispenser.cpp
  └── infrastructure/adapters/SimulatedPaymentGateway.cpp
```

Application層とDomain層は、具体的な実装に依存せず、抽象化されたインターフェースにのみ依存します。

### 2. ドメイン駆動設計（DDD）の要素

- **Value Objects**: Money, Price, Quantity等で不変性を表現し、ビジネスロジックを一貫して管理
- **Entities**: Product, ProductSlot等でアイデンティティを持つオブジェクトを定義
- **Aggregates**: Inventory, Wallet, Sales等でビジネスルールを集約
- **Domain Services**: PurchaseEligibilityService等で複雑なビジネスロジックを実装

### 3. Facadeパターン（VendingMachineApplication）

単一の統合入口点として機能し、DI（依存性注入）の役割も果たします：

```cpp
// 使用例
auto app = VendingMachineApplication(
    coin_mech, dispenser, payment_gateway, transaction_history
);

// ユースケースの取得
auto& purchase_with_cash = app.getPurchaseWithCashUseCase();
auto& inventory_refill = app.getInventoryRefillUseCase();
```

### 4. リポジトリパターン

永続化の抽象化で、実装の詳細を隠蔽：

```cpp
// アプリケーション層で定義
class ITransactionHistoryRepository {
  virtual void save(const TransactionRecord& record) = 0;
  virtual std::vector<TransactionRecord> getAll() const = 0;
  // ...
};

// インフラ層で実装
class InMemoryTransactionHistoryRepository : 
    public ITransactionHistoryRepository {
  // 実装
};
```

### 5. イベントストーミング対応

購入フローは、ユビキタス言語で定義されたイベントストーミングに完全に対応しています。

#### 現金購入の処理順序（PurchaseWithCashUseCase）

```
Step 1: 商品選択（Sales集約）
Step 2: 商品情報取得
Step 3: 決済待ち状態に遷移
Step 4: 在庫減算（イベントストーミング Step 5）
Step 5: 排出中状態に遷移（Step 6準備）
Step 6: 商品排出（イベントストーミング Step 6）
Step 7: 決済確定（イベントストーミング Step 7）← wallet_.withdraw()
Step 8: お釣り返却（イベントストーミング Step 8）
Step 9: トランザクション完了
Step 10: トランザクション履歴を記録

失敗時: inventory_.refill() でロールバック
```

#### 電子決済の処理順序（PurchaseWithEMoneyUseCase）

```
Step 1-4: 同上
Step 5: 外部決済ゲートウェイに決済要求
Step 6: 決済ステータス確認

決済成功時:
  Step 6: 排出中状態に遷移
  Step 7: 商品排出
  Step 8: 決済確定（wallet_.withdraw()）
  Step 9: トランザクション完了
  Step 10: トランザクション履歴を記録

決済失敗時:
  在庫ロールバック（inventory_.refill()）
  セッションキャンセル
```

---

## 実装されたユースケース

### ✅ 現金購入 (PurchaseWithCashUseCase)

- 投入金額の管理
- 在庫確認と商品選択
- 決済処理と在庫更新
- お釣り返却
- ロールバック機構

**テスト**: 10個のテストケース

### ✅ 電子決済 (PurchaseWithEMoneyUseCase)

- 外部決済ゲートウェイとの連携
- 非同期決済対応
- 決済失敗時のロールバック
- 複数商品の連続購入

**テスト**: 10個のテストケース

### ✅ 在庫補充 (InventoryRefillUseCase)

- スロット別の在庫補充
- 最大容量チェック
- 複数スロットの一括補充

**テスト**: 6個のテストケース

### ✅ 売上回収 (CashCollectionUseCase)

- 累計売上集計
- トランザクション履歴クリア
- 複数回の回収管理

**テスト**: 6個のテストケース

### ✅ 売上レポート (SalesReportingUseCase)

- スロット別売上
- 決済方法別集計
- 総売上・取引数集計

**テスト**: 7個のテストケース

---

## テスト状況

**合計: 237テスト、すべて合格 ✅**

### テスト構成

| カテゴリ | テスト数 |
|---------|--------|
| ユースケース | 33 |
| ドメインオブジェクト | 165 |
| インフラ層 | 10 |
| **合計** | **237** |

### テスト実行方法

```bash
# 全テスト実行
cd build
make test

# または
./test/unit_tests

# 特定のテストスイート
./test/unit_tests --gtest_filter="PurchaseWithCashUseCaseTest.*"
./test/unit_tests --gtest_filter="InventoryRefillUseCaseTest.*"
```

---

## ビルド・実行方法

### ビルド

```bash
# ディレクトリ構成
onion_architect/
├── CMakeLists.txt
├── build/  ← ビルド時に作成
└── src/

# ビルド手順
cd vending_machine/onion_architect

# ビルドディレクトリの作成とビルド
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j4

# テスト実行
cd build
./test/unit_tests

# 実行
./vending_machine
```

### クリーンビルド

```bash
cd vending_machine/onion_architect
rm -rf build
cmake -S . -B build
cmake --build build -j4
```

---

## 設計パターン

### 1. Value Object パターン

ビジネスルールを内包した不変オブジェクト：

- `Money` - 金額計算と負の値チェック
- `Price` - 商品価格
- `Quantity` - 在庫数と最大値チェック
- `ProductInfo` - 商品情報の一体管理

### 2. Aggregate パターン

ビジネスルールの一貫性を保証する集約：

- `Inventory` - 在庫管理の全責務
- `Wallet` - 通貨管理の全責務
- `Sales` - 販売セッションの全責務

### 3. Entity パターン

識別子を持つドメインオブジェクト：

- `ProductSlot` - 特定のスロットを表現
- `TransactionSession` - 取引セッションを表現

### 4. Domain Service パターン

複数の集約を跨ぐロジック：

- `PurchaseEligibilityService` - 購入可能判定

### 5. Repository パターン

永続化の抽象化：

- `ITransactionHistoryRepository` - インターフェース
- `InMemoryTransactionHistoryRepository` - メモリベース実装

### 6. Facade パターン

複雑な依存関係を隠蔽：

- `VendingMachineApplication` - 統合入口点

---

## DDD原則の適用

### ユビキタス言語

ドメイン専門家とエンジニアが共通の言語を使用：

| ドメイン用語 | 実装クラス | 意味 |
|-------------|----------|------|
| 販売管理 | Sales | 取引ライフサイクル管理 |
| 通貨管理 | Wallet | 残高・決済管理 |
| 在庫管理 | Inventory | スロット管理 |
| 商品 | ProductSlot | スロット内の商品 |
| 取引 | TransactionSession | 一連の購入 |

### イベントストーミング

ビジネスプロセスをイベント駆動で定義：

- 入金イベント → 残高更新
- 商品選択 → 販売可能判定
- 在庫減算 → 商品排出
- 決済 → お釣り返却

### 集約

トランザクション境界を明確に定義：

1. **Inventory** - 商品の在庫状態を保証
2. **Wallet** - 通貨情報の一貫性を保証
3. **Sales** - 取引セッションの状態遷移を保証

---

## 主な実装の工夫

### 1. インターフェースの定義位置

**ドメイン層に定義**（DIP準拠）：
```cpp
// domain/interfaces/ - インターフェース定義
class ICoinMech { /* ... */ };
class IDispenser { /* ... */ };
class IPaymentGateway { /* ... */ };
```

**インフラ層で実装**：
```cpp
// infrastructure/adapters/ - 実装
class SimulatedCoinMech : public ICoinMech { /* ... */ };
class SimulatedDispenser : public IDispenser { /* ... */ };
class SimulatedPaymentGateway : public IPaymentGateway { /* ... */ };
```

### 2. ロールバック機構

決済失敗時に在庫を復元：

```cpp
try {
  // ステップ実行
  inventory_.dispense(slot_id);
  // ... 決済処理
  wallet_.withdraw(payment);
} catch (...) {
  // 決済失敗時のロールバック
  inventory_.refill(slot_id, Quantity(1));
  throw;
}
```

### 3. ConsoleUI

すべての実装詳細をマスク：

```cpp
ConsoleUI(application::VendingMachineApplication &app);
```

- VendingMachineApplicationのみに依存
- 各ユースケースの詳細に依存しない

---

## 拡張ポイント

### 1. 新しい決済方法の追加

`IPaymentGateway`の実装を追加するだけで対応可能：

```cpp
class CreditCardGateway : public IPaymentGateway {
  // 実装
};
```

### 2. 永続化の変更

`ITransactionHistoryRepository`の実装を切り替え：

```cpp
// ファイルベース実装
class FileTransactionHistoryRepository : 
    public ITransactionHistoryRepository { /* ... */ };

// データベース実装
class DatabaseTransactionHistoryRepository : 
    public ITransactionHistoryRepository { /* ... */ };
```

### 3. 新しいユースケースの追加

`application/usecases/`に新しい`UseCase`クラスを追加：

```cpp
class PriceChangeUseCase {
  bool changePrice(SlotId slot_id, Price new_price);
};
```

---

## 依存性の方向

本実装の依存関係図：

```
Presentation Layer
    ↓ 依存
Application Layer
    ↓ 依存
Domain Layer (Interface Only)
    ↑ 実装 (DIP)
Infrastructure Layer (Concrete Implementation)
```

### 通常のLayered Architectureとの違い

| 特性 | 通常のLayered | 本実装（オニオン） |
|------|--------------|---------------|
| 依存方向 | 上→下（線形） | 上→下、Infrastructure←Domain |
| インターフェース定義位置 | Infrastructure | Domain |
| 実装 | Infrastructure | Infrastructure |
| Domain層の外部依存 | 少ない | なし |
| テスト容易性 | 中程度 | 高い |

### 実装例

**Domain層（インターフェース定義）:**
```cpp
// domain/interfaces/ICoinMech.hpp
class ICoinMech {
  virtual bool canDispenseChange(const Money& amount) = 0;
  virtual void dispenseChange(const Money& amount) = 0;
  virtual ~ICoinMech() = default;
};
```

**Infrastructure層（実装）:**
```cpp
// infrastructure/adapters/SimulatedCoinMech.hpp
class SimulatedCoinMech : public ICoinMech {
  bool canDispenseChange(const Money& amount) const override;
  void dispenseChange(const Money& amount) override;
};
```

**Application層（インターフェースへの依存）:**
```cpp
// application/VendingMachineApplication.hpp
class VendingMachineApplication {
  VendingMachineApplication(
      domain::ICoinMech& coin_mech,  // インターフェースに依存
      domain::IDispenser& dispenser,
      domain::IPaymentGateway& payment_gateway,
      application::ITransactionHistoryRepository& transaction_history
  );
};
```

**重要**: Domain層が `ICoinMech` インターフェースを定義し、Infrastructure層が `SimulatedCoinMech` の具体的な実装を提供します。Application層はDomain層のインターフェースに依存します。

---

## 初学者向けのアーキテクチャ学習ガイド

### 各層の理解

**Presentation層（表現層）**: 
- ユーザーとのやり取り（UIやAPI）
- 例: ConsoleUI

**Application層（アプリケーション層）**:
- ビジネス手続きの調整（ユースケース）
- 例: PurchaseWithCashUseCase

**Domain層（ドメイン層）**:
- ビジネスロジック（何をするのか）
- インターフェース定義（hardware dependencies）
- 例: Inventory, Wallet, Sales, ICoinMech

**Infrastructure層（インフラ層）**:
- Domain層で定義されたインターフェースを実装
- 外部システムとの連携
- 例: SimulatedCoinMech, InMemoryRepository

### 依存関係の原則（DIP）

```
✓ Domain層がインターフェースを定義（OK）
  - ICoinMech, IDispenser, IPaymentGateway
  
✓ Infrastructure層が実装（OK）
  - SimulatedCoinMech, SimulatedDispenser, SimulatedPaymentGateway
  
✓ Application層がインターフェースに依存（OK）
  - ユースケースが domain::ICoinMech を使用
  
✗ Domain層がInfrastructure層に依存しない（NG）
  - Domain層は具体実装を知らない
```

### 新機能追加時の流れ

1. **Domain層**: ビジネスロジックを追加（Aggregate、ValueObject等）
2. **Application層**: インターフェースを定義（if needed）
3. **Application層**: ユースケースを実装
4. **Infrastructure層**: 具体的な実装を追加
5. **Presentation層**: UIやAPIを追加

### テストの構造

```
テストファイル               被テスト対象
test/application/usecases/  → src/application/usecases/
test/domain/                → src/domain/
test/infrastructure/        → src/infrastructure/
```

各層のテストは独立している状態で書く（モックを使用）。

---

## パフォーマンス

- **ビルド時間**: 約10秒（クリーンビルド）
- **テスト実行時間**: 約27ms（237テスト）
- **実行ファイルサイズ**: 約2.3MB

---

## まとめ

このオニオンアーキテクチャ実装は、DIP（依存性逆転の原則）を適用した設計です：

✅ **DIP適用** - Domain層がインターフェースを定義
✅ **DDD準拠** - ユビキタス言語、集約、値オブジェクト
✅ **イベントストーミング対応** - ビジネスフローを正確に実装
✅ **Facade/DI** - シンプルで保守しやすいAPI
✅ **テスト駆動** - 237テストすべて合格
✅ **教育的** - 明確な層の分離と責務の分割
✅ **拡張性** - 新規ユースケース追加が容易

シンプルでありながら、DDD原則とアーキテクチャパターンを実践的に学べる実装例となっています。
