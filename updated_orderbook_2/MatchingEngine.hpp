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

        struct RiskLimits
        {
            double maxPrice{ 1'000'000.0 };
            uint64_t maxQuantity{ 1'000'000 };
            bool allowMarketOrders{ true };
        };

        MatchingEngine();

        // Submit order (auto ID generation)
        uint64_t submitOrder(Side side,
            OrderType type,
            double price,
            uint64_t quantity);

        void setRiskLimits(RiskLimits limits) noexcept;

        const RiskLimits& getRiskLimits() const noexcept;

        // Access trades
        const std::vector<Trade>& getTrades() const;

        // Market data access
        void printTopOfBook() const;
        void printFullDepth() const;
        const OrderBook& getOrderBook() const noexcept;

        void reset();

    private:

        OrderBook orderBook;

        // Thread-safe ID generator
        std::atomic<uint64_t> nextOrderId;
        RiskLimits riskLimits;

        bool validateSubmission(OrderType type,
            double price,
            uint64_t quantity) const noexcept;

    };

} // namespace hft
