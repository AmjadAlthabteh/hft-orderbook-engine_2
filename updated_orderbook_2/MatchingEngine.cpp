#include "MatchingEngine.hpp"
#include "HFTUtils.hpp"
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

        if (!validateSubmission(type, price, quantity))
            return 0;

        uint64_t orderId = nextOrderId.fetch_add(1, std::memory_order_relaxed);

        Order order(orderId, side, type, price, quantity);

        orderBook.addOrder(std::move(order));

        return orderId;
    }

    void MatchingEngine::setRiskLimits(RiskLimits limits) noexcept
    {
        riskLimits = limits;
    }

    const MatchingEngine::RiskLimits& MatchingEngine::getRiskLimits() const noexcept
    {
        return riskLimits;
    }

    bool MatchingEngine::validateSubmission(OrderType type,
        double price,
        uint64_t quantity) const noexcept
    {
        if (type == OrderType::Market)
        {
            return riskLimits.allowMarketOrders
                && quantity > 0
                && quantity <= riskLimits.maxQuantity;
        }

        return validateOrder(price,
            quantity,
            riskLimits.maxPrice,
            riskLimits.maxQuantity);
    }

    const std::vector<Trade>& MatchingEngine::getTrades() const
    {
        return orderBook.getTrades();
    }

  

    void MatchingEngine::printTopOfBook() const
    {
        orderBook.printTopOfBook();
    }

    void MatchingEngine::printFullDepth() const
    {
        orderBook.printFullDepth();
    }

    const OrderBook& MatchingEngine::getOrderBook() const noexcept
    {
        return orderBook;
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
        nextOrderId.store(1, std::memory_order_relaxed);
    }

} // namespace hft
