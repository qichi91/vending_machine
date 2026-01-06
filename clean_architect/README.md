# Clean Architecture - 自動販売機システム

このディレクトリは、**クリーンアーキテクチャ（Clean Architecture）** で実装された自動販売機システムです。

## 概要

クリーンアーキテクチャは、関心事の分離（Separation of Concerns）を徹底し、ビジネスロジックを中心に据えることで、フレームワークやUI、データベースなどの外部要素から独立した保守性の高いシステムを構築するための設計手法です。

同心円状のレイヤー構造を持ち、**依存関係のルール（Dependency Rule）** により、ソースコードの依存は常に外側から内側に向かいます。

```
                   Frameworks & Drivers Layer
                 (UI, DB, Web, Devices, External)
                            ↓
               Interface Adapters Layer
           (Controllers, Gateways, Presenters)
                            ↓
               Application Business Rules
                     (Use Cases)
                            ↓
┌────────────────────────────────────────────────────────┐
│             Enterprise Business Rules                  │
│                    (Entities)                          │
│                                                        │
│   Entities, Value Objects, Domain Services             │
│   Enterprise Business Logic                            │
└────────────────────────────────────────────────────────┘
```

### 依存関係の流れ（Dependency Rule）

```
Frameworks & Drivers → Interface Adapters → Use Cases → Entities
```

**各層の責務：**
- **Entities (Domain)**: 最も内側の層。ビジネス全体に共通するルール（エンティティ、値オブジェクトなど）。外部の変更に影響されない。
- **Use Cases**: アプリケーション固有のビジネスルール。エンティティ間のデータの流れを調整し、ユースケースを達成する。
- **Interface Adapters**: データ形式の変換を行う。ユースケースやエンティティに最も便利な形式と、データベースやWebなどの外部エージェンシーに最も便利な形式との間の変換を担う。
- **Frameworks & Drivers**: 最も外側の層。フレームワーク、ツール、データベース、UIなどの詳細。

---

## ディレクトリ構成

```
clean_architect/
├── CMakeLists.txt                          # CMake設定ファイル
├── src/
│   ├── main.cpp                             # エントリーポイント
│   ├── frameworks_drivers/                  # Frameworks & Drivers Layer
│   │   └── ui/
│   │       ├── ConsoleUI.hpp               # UI定義
│   │       └── ConsoleUI.cpp               # UI実装
│   ├── interface_adapters/                  # Interface Adapters Layer
│   │   ├── controllers/
│   │   │   └── (Controllers implemention)
│   │   └── gateways/
│   │       └── (Gateways implementation)
│   ├── usecases/                            # Use Cases Layer (Application Business Rules)
│   │   ├── PurchaseWithCashUseCase.hpp
│   │   ├── PurchaseWithCashUseCase.cpp
│   │   ├── PurchaseWithEMoneyUseCase.hpp
│   │   ├── (Other Use Cases...)
│   │   ├── dto/                            # Data Transfer Objects
│   │   └── (Interfaces for Output Ports)
│   └── domain/                              # Entities Layer (Enterprise Business Rules)
│       ├── common/
│       │   ├── Money.hpp
│       │   ├── Price.hpp
│       │   └── Quantity.hpp
│       ├── inventory/
│       │   ├── Inventory.hpp
│       │   └── ProductSlot.hpp
│       ├── payment/
│       │   └── Wallet.hpp
│       ├── sales/
│       │   └── Sales.hpp
│       ├── services/
│       │   └── PurchaseEligibilityService.hpp
│       ├── interfaces/                     # ★ 依存性逆転のためのインターフェース定義（Use Casesが必要とする）
│       │   ├── ICoinMech.hpp
│       │   ├── IDispenser.hpp
│       │   └── IPaymentGateway.hpp
│       └── repositories/
│           └── ITransactionHistoryRepository.hpp
├── test/                                    # テストスイート
│   ├── domain/
│   ├── usecases/
│   └── interface_adapters/
└── build/                                   # ビルド出力ディレクトリ
```

---

## アーキテクチャの特徴

### 1. 依存性逆転原則（DIP）と依存関係のルール

内側のレイヤー（Entities, Use Cases）は何ら外側のレイヤー（Interface Adapters, Frameworks & Drivers）について知ることはありません。

例えば、`Use Cases` 層がハードウェアを制御したい場合、直接呼び出すのではなく、インターフェースを定義し、外側の層（Interface Adapters や Frameworks & Drivers）がそのインターフェースを実装します。

```cpp
// domain/interfaces/ICoinMech.hpp (内側の層で定義)
class ICoinMech {
public:
    virtual void insertCoin(const Money& amount) = 0;
    // ...
};

// interface_adapters/gateways/SimulatedCoinMech.hpp (外側の層で実装)
class SimulatedCoinMech : public domain::ICoinMech {
    // 具体的な実装
};
```

### 2. ドメイン駆動設計（DDD）のアプローチ

最も内側の `domain` ディレクトリ（Entities Layer）では、DDDの概念を用いてリッチなドメインモデルを構築しています。

- **Entities**: ビジネスロジックと状態を持つオブジェクト（例：Inventory, Sales）
- **Value Objects**: 不変の値（例：Money, Price）
- **Domain Services**: エンティティに属さないロジック（例：PurchaseEligibilityService）

### 3. テスト容易性

ビジネスロジック（Use Cases, Entities）はUIやDB、フレームワークに依存しないため、これらをモック化することで簡単に単体テストが可能です。

---

## 主要なユースケース

### ✅ 現金での購入 (PurchaseWithCashUseCase)
現金投入、商品選択、在庫確認、商品排出、お釣り返却の一連の流れを制御します。

### ✅ 電子マネーでの購入 (PurchaseWithEMoneyUseCase)
外部決済ゲートウェイを用いた非同期的な決済プロセスを扱います。

### ✅ 在庫補充 (InventoryRefillUseCase)
メンテナンス係による商品の補充処理を行います。

---

## ビルドと実行

### 必要条件
- C++17 以上
- CMake 3.14 以上

### ビルド手順

```bash
# プロジェクトルートから
cd vending_machine/clean_architect

# ビルドディレクトリの作成
mkdir -p build
cd build

# CMakeの実行
cmake ..

# ビルド
make
```

### 実行方法

```bash
# ビルドディレクトリ内で
./vending_machine
```

### テストの実行

```bash
# ビルドディレクトリ内で
ctest --output-on-failure
```

---

## まとめ

この実装は、クリーンアーキテクチャの原則に忠実に従い、各レイヤーの責務を明確に分離しています。

- **独立性**: ビジネスロジックはUIや外部デバイスから完全に独立しています。
- **テスタビリティ**: Use Cases レイヤーに対するテストが容易に記述できています。
- **交換可能性**: 例えばコンソールUIをWeb UIに置き換える場合も、`frameworks_drivers` 層の変更だけで済み、ビジネスロジックへの影響はありません。
