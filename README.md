# FLOB — Fast Limit Order Book Engine

High-performance C++ limit order book engine built for quantitative research, with microsecond-level matching, realistic
order types, and strict price-time priority.

## Overview

FLOB is a lightweight research and educational project that implements a high-performance *limit order book* (LOB) and
matching engine.
It focuses on the core mechanics of electronic order matching — emphasizing microsecond-level performance, price-time
priority, and realistic order behavior used in quantitative trading systems.

## Features

## Features

### Core
- Modular **logging system** for diagnostics and performance tracing
- Custom **vector container** optimized for performance
- **Intrusive reference counting** for lightweight memory management (replaces `std::shared_ptr`)
- Built-in **UUID implementation** for unique order identifiers

### Order Book
- **Order types**: `GoodTillCancel`, `ImmediateOrCancel`, and `GoodForDay`
- **Matching algorithm**: strict price–time priority (FIFO within price levels)
- **Data structures**: $O(1)$ order lookup and $O(\log n)$ price-level access using efficient maps
- **Trade execution**: automatic order matching with support for partial fills

## Build

### Prerequisites

- CMake >= 3.30
- C++23
- Compiler: Clang / GCC / MSVC with `<print>` and `<format>` support

### Build & Run

```shell
mkdir build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j

./binaries/release/example
```

## Roadmap

- **Additional time-in-force** — add `GoodTillTime` (GTT) and `FillOrKill` (FOK) order types
- **Advanced order types** such as *iceberg*, *hidden*, and *post-only* orders
- **Market impact and execution cost modeling** to evaluate slippage and liquidity effects
- **Position and risk management layer** to track exposure and PnL during strategy evaluation
- **Historical data replay and backtesting framework** for deterministic market simulation

## Research Context

FLOB is intended for:

- studying limit order book dynamics
- modeling execution strategies
- teaching market microstructure concepts
- benchmarking matching engine algorithms

## License

MIT License — free for research, education, and experimentation.