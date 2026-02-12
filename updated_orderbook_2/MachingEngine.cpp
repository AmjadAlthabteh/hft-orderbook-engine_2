#include "MatchingEngine.hpp"
#include <algorithm>
#include <iostream>

/*
    match()

    Executes trades while:
    bestBid >= bestAsk

    - FIFO preserved via deque
    - No printing in hot loop
    - Aggregates trade statistics
*/
void MatchingEngine::match() noexcept
{
    auto& bids = book.getBids();
    auto& asks = book.getAsks();

    while (!bids.empty() && !asks.empty())
    {
        auto& [bidPrice, bidLevel] = *bids.begin();
        auto& [askPrice, askLevel] = *asks.begin();

        auto& bestBid = bidLevel.front();
        auto& bestAsk = askLevel.front();

        // Stop if spread does not cross
        if (bestBid.price < bestAsk.price)
            break;

        OrderBook::Quantity tradeQty =
            std::min(bestBid.quantity, bestAsk.quantity);

        // Update stats (NO printing here)
        totalTrades++;
        totalVolume += tradeQty;
        totalNotional += tradeQty * askPrice;

        // Reduce quantities
        bestBid.quantity -= tradeQty;
        bestAsk.quantity -= tradeQty;

        // Remove filled orders
        if (bestBid.quantity == 0)
        {
            bidLevel.pop_front();
            if (bidLevel.empty())
                bids.erase(bidPrice);
        }

        if (bestAsk.quantity == 0)
        {
            askLevel.pop_front();
            if (askLevel.empty())
                asks.erase(askPrice);
        }
    }
}

/*
    Resets statistics (useful between benchmark runs)
*/
void MatchingEngine::resetStats() noexcept
{
    totalTrades = 0;
    totalVolume = 0;
    totalNotional = 0.0;
}

/*
    Volume Weighted Average Price
*/
double MatchingEngine::vwap() const noexcept
{
    if (totalVolume == 0)
        return 0.0;

    return totalNotional / static_cast<double>(totalVolume);
}

/*
    Clean output summary
*/
void MatchingEngine::printSummary() const noexcept
{
    std::cout << "Total Trades   : " << totalTrades << "\n";
    std::cout << "Total Volume   : " << totalVolume << "\n";
    std::cout << "VWAP           : " << vwap() << "\n";
}
