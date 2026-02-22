#include "MatchingEngine.hpp"
#include <iostream>

namespace hft
{

    // ============================================================
    // CONSTRUCTOR

    MatchingEngine::MatchingEngine()
        : nextOrderId(1)   // Start IDs at 1
    {
    }

    

    uint64_t MatchingEngine::submitOrder(Side side,
        OrderType type,
        double price,
        uint64_t quantity)
    {
        /*
            This function acts as the public API
            for submitting new orders into the system.

            In a real exchange this would:
                - Validate risk
                - Check margin
                - Validate price bands
                - Timestamp at gateway
                - Pass to matching engine thread
        */

        uint64_t orderId = nextOrderId.fetch_add(1, std::memory_order_relaxed);

        Order order(orderId, side, type, price, quantity);

        orderBook.addOrder(order);

        // Pull latest trades from book
        tradeCache = orderBook.match();

        return orderId;
    }

    const std::vector<Trade>& MatchingEngine::getTrades() const
    {
        return tradeCache;
    }

  

    void MatchingEngine::printTopOfBook() const
    {
        orderBook.printTopOfBook();
    }

    void MatchingEngine::printFullDepth() const
    {
        orderBook.printFullDepth();
    }


    void MatchingEngine::reset()
    {
        /*
            Clears:
                - Order book
                - Trade history
                - Resets order IDs

            Useful for:
                - Backtesting
                - Simulation runs
        */

        orderBook.clear();
        tradeCache.clear();
        nextOrderId.store(1, std::memory_order_relaxed);
    }

} // namespace hft