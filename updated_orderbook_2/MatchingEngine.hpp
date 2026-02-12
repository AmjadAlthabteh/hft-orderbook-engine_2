#pragma once

#include "OrderBook.hpp"
#include <cstdint>

/*
    MatchingEngine

    Responsible for:
    - Price-time priority matching
    - Trade aggregation
    - Performance-safe execution (no I/O in hot path)
*/

class MatchingEngine {
private:
    OrderBook& book;

    // ---- Trade Statistics ----
    std::uint64_t totalTrades{ 0 };
    std::uint64_t totalVolume{ 0 };
    double totalNotional{ 0.0 };   // sum(price * quantity)

public:
    explicit MatchingEngine(OrderBook& ob) noexcept
        : book(ob) {
    }

    void match() noexcept;

    void resetStats() noexcept;

    void printSummary() const noexcept;

    [[nodiscard]]
    double vwap() const noexcept;
};
