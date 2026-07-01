# updated_orderbook_2

# hft Order Book Engine

a modular C++20 price-time FIFO matching engine with performance benchmarking tools.

Features:
- Limit order book
- Matching engine
- Market and limit order support
- Configurable submission risk limits
- VWAP calculation
- Benchmark utilities
- Cache-aligned structures
- No-copy trade history access
- CMake and Visual Studio build support
- Modern C++20 design

## Build

```powershell
cmake -S updated_orderbook_2 -B build -G Ninja
cmake --build build
```

## Run

```powershell
.\build\updated_orderbook_2.exe
```

## Test

```powershell
cmake --build build --target updated_orderbook_tests
.\build\updated_orderbook_tests.exe
```
