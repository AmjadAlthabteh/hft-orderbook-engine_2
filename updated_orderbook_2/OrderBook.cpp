#include "OrderBook.hpp"


//Implemented a price-time priority matching engine in modern C++ supporting depth aggregation, partial //fills, and VWAP tracking with exchange-realistic trade execution logic.

namespace hft
{

    void OrderBook::addOrder(const Order& order)
    {
        if (order.side == Side::Buy)
        {
            bids[order.price].addOrder(order);
        }
        else
        {
            asks[order.price].addOrder(order);
        }

        // Immediately attempt matching after insertion
        matchOrders();
    }

    // ============================================================
    // MATCH ORDERS (CORE ENGINE LOGIC)
    // ============================================================

    void OrderBook::matchOrders()
    {
        /*
            Matching logic:

            While:
                bestBid >= bestAsk

            Trade occurs at ask price (standard exchange model)

            Price-time priority:
                - First price level
                - FIFO within level
        */

        while (!bids.empty() && !asks.empty())
        {
            auto bestBidIt = bids.begin();
            auto bestAskIt = asks.begin();

            double bestBid = bestBidIt->first;
            double bestAsk = bestAskIt->first;

            if (bestBid < bestAsk)
                break;  // No crossing market

            PriceLevel& bidLevel = bestBidIt->second;
            PriceLevel& askLevel = bestAskIt->second;

            Order& buyOrder = bidLevel.orders.front();
            Order& sellOrder = askLevel.orders.front();

            uint64_t tradeQty = std::min(buyOrder.quantity, sellOrder.quantity);

            trades.push_back({
                buyOrder.id,
                sellOrder.id,
                bestAsk,   // trade at ask price
                tradeQty,
                std::chrono::steady_clock::now()
                });

            bidLevel.reduceFront(tradeQty);
            askLevel.reduceFront(tradeQty);

            if (bidLevel.empty())
                bids.erase(bestBidIt);

            if (askLevel.empty())
                asks.erase(bestAskIt);
        }
    }

    // ============================================================
    // RETURN TRADES
    // ============================================================

    std::vector<Trade> OrderBook::match()
    {
        return trades;
    }

    // ============================================================
    // MARKET DATA
    // ============================================================

    double OrderBook::getBestBid() const
    {
        return bids.empty() ? 0.0 : bids.begin()->first;
    }

    double OrderBook::getBestAsk() const
    {
        return asks.empty() ? 0.0 : asks.begin()->first;
    }

    double OrderBook::getSpread() const
    {
        if (bids.empty() || asks.empty())
            return 0.0;

        return getBestAsk() - getBestBid();
    }

    double OrderBook::getMidPrice() const
    {
        if (bids.empty() || asks.empty())
            return 0.0;

        return (getBestAsk() + getBestBid()) / 2.0;
    }

    uint64_t OrderBook::getTotalBidVolume() const
    {
        uint64_t total = 0;
        for (const auto& [price, level] : bids)
            total += level.totalVolume;

        return total;
    }

    uint64_t OrderBook::getTotalAskVolume() const
    {
        uint64_t total = 0;
        for (const auto& [price, level] : asks)
            total += level.totalVolume;

        return total;
    }

    double OrderBook::calculateVWAP() const
    {
        double totalValue = 0.0;
        uint64_t totalVolume = 0;

        for (const auto& trade : trades)
        {
            totalValue += trade.price * trade.quantity;
            totalVolume += trade.quantity;
        }

        if (totalVolume == 0)
            return 0.0;

        return totalValue / totalVolume;
    }

    // ============================================================
    // OUTPUT
    // ============================================================

    void OrderBook::printTopOfBook() const
    {
        std::cout << "\n--- TOP OF BOOK ---\n";
        std::cout << "Best Bid: " << getBestBid() << "\n";
        std::cout << "Best Ask: " << getBestAsk() << "\n";
        std::cout << "Spread:   " << getSpread() << "\n";
        std::cout << "MidPrice: " << getMidPrice() << "\n";
    }

    void OrderBook::printFullDepth() const
    {
        std::cout << "\n===== ORDER BOOK DEPTH =====\n";

        std::cout << "\nASKS:\n";
        for (const auto& [price, level] : asks)
            std::cout << price << " | " << level.totalVolume << "\n";

        std::cout << "\nBIDS:\n";
        for (const auto& [price, level] : bids)
            std::cout << price << " | " << level.totalVolume << "\n";
    }


    bool OrderBook::empty() const
    {
        return bids.empty() && asks.empty();
    }

    void OrderBook::clear()
    {
        bids.clear();
        asks.clear();
        trades.clear();
    }

} // namespace hft