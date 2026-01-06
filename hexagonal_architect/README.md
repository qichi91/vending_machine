# Hexagonal Architecture - 自動販売機システム

このディレクトリは、**ヘキサゴナルアーキテクチャ（Hexagonal Architecture / Ports and Adapters）** で実装された自動販売機システムです。

## 概要

ヘキサゴナルアーキテクチャは、アプリケーションを中心に置き、外部との通信を「ポート（Ports）」と「アダプター（Adapters）」を通じて行うことで、アプリケーションを外部の技術的詳細（UI、データベース、外部APIなど）から隔離する設計パターンです。

```
        Adapter (UI)         Adapter (Test)
             ↓                    ↓
      ┌───────────  Ports  ───────────┐
      │           (Inbound)           │
      │                               │
      │        APPLICATION CORE       │
      │                               │
      │           (Outbound)          │
      └───────────  Ports  ───────────┘
             ↓                    ↓
       Adapter (DB)       Adapter (Payment)
```

### 構成要素

- **Application Core**: ビジネスロジック。ドメインモデルやアプリケーションサービスが含まれます。
- **Ports (ポート)**: 
    - **Inbound (Driving) Ports**: 外部からアプリケーションを操作するためのインターフェース（API定義など）。
    - **Outbound (Driven) Ports**: アプリケーションが外部リソースを利用するためのインターフェース（リポジトリ定義など）。
- **Adapters (アダプター)**:
    - **Primary (Driving) Adapters**: Inbound Portを使ってアプリケーションを駆動するもの（CLI、Webコントローラ、テスト）。
    - **Secondary (Driven) Adapters**: Outbound Portを実装し、外部システムへの接続を提供するもの（InMemoryDB、Mock決済システム）。

---

## ディレクトリ構成

```
hexagonal_architect/
├── CMakeLists.txt
├── src/
│   ├── main.cpp                         # エントリーポイント (Wire-up)
│   ├── domain/                          # ビジネスロジックの中心 (Entities, Value Objects)
│   ├── application/                     # Application Services (Inbound Portの実装)
│   ├── ports/                           # インターフェース定義
│   │   ├── inbound/                     # Driving Ports (Use Cases interfaces)
│   │   └── outbound/                    # Driven Ports (Infrastructure interfaces)
│   └── adapters/                        # 具体的な実装
        ├── inbound/                     # Driving Adapters (e.g., Console UI)
        └── outbound/                    # Driven Adapters (e.g., Repositories)
└── test/                                # テスト
```

## 依存関係のルール

依存の方向は常に **外側（Adapters）から内側（Ports/Application/Domain）** に向かいます。

`Adapters` -> `Ports` <-> `Application` -> `Domain`

アプリケーションコア（Application/Domain）は、アダプター（具体的なUIやDB）について一切知りません。

---

## ビルドと実行

### 必要条件
- C++17 以上
- CMake 3.14 以上

### ビルド

```bash
cd vending_machine/hexagonal_architect
mkdir -p build && cd build
cmake ..
make
```

### 実行

```bash
./vending_machine
```

### テスト

```bash
ctest --output-on-failure
```
