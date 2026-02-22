#pragma once

#include "OrderBook.hpp"
#include <vector>

/*

    This layer performs analytical computations on the
    order book and trade data.

    It does NOT modify the book.
    It analyzes it.

    Useful for:
        - Market microstructure research
        - Backtesting
        - Strategy signals
        - Engine validation
*/

namespace hft
{

    class HFTAlgorithms
    {
    public:

        // Order book imbalance (Bid vs Ask pressure)
        static double computeOrderImbalance(const OrderBook& book);

        // Spread as percentage of mid
        static double computeSpreadPercentage(const OrderBook& book);

        // Simple momentum from recent trades
        static double computeMomentum(const std::vector<Trade>& trades,
            size_t lookback);

        // Rolling average trade price
        static double computeRollingAverage(const std::vector<Trade>& trades,
            size_t lookback);

        // Print analytics summary
        static void printAnalytics(const OrderBook& book,
            const std::vector<Trade>& trades);
    };

} // namespace hft