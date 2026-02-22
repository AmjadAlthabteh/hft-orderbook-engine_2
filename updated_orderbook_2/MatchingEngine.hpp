#pragma once

#include "OrderBook.hpp"
#include <atomic>
#include <vector>

/*
* 

    Responsibilities:

    Order submission interface
    Order ID generation
   Interaction with OrderBook
    Trade reporting
    High-level control layer

    This sits above OrderBook and would normally
    connect to:
        - Risk Manager
        - Network Gateway
        - Logging system
        - Market Data Feed
*/

namespace hft
{

    class MatchingEngine
    {
    public:

        MatchingEngine();

        // Submit order (auto ID generation)
        uint64_t submitOrder(Side side,
            OrderType type,
            double price,
            uint64_t quantity);

        // Access trades
        const std::vector<Trade>& getTrades() const;

        // Market data access
        void printTopOfBook() const;
        void printFullDepth() const;

        void reset();

    private:

        OrderBook orderBook;

        // Thread-safe ID generator
        std::atomic<uint64_t> nextOrderId;

        std::vector<Trade> tradeCache;
    };

} // namespace hft