#include "HFTAlgorithms.hpp"
#include <iostream>
#include <algorithm>

namespace hft
{

    // ============================================================
    // ORDER IMBALANCE
    // ============================================================

    double HFTAlgorithms::computeOrderImbalance(const OrderBook& book)
    {
        uint64_t bidVol = book.getTotalBidVolume();
        uint64_t askVol = book.getTotalAskVolume();

        if (bidVol + askVol == 0)
            return 0.0;

        return static_cast<double>(bidVol - askVol) /
            static_cast<double>(bidVol + askVol);
    }

    // ============================================================
    // SPREAD PERCENTAGE
    // ============================================================

    double HFTAlgorithms::computeSpreadPercentage(const OrderBook& book)
    {
        double spread = book.getSpread();
        double mid = book.getMidPrice();

        if (mid == 0.0)
            return 0.0;

        return (spread / mid) * 100.0;
    }

    // ============================================================
    // MOMENTUM (Recent Trade Bias)
    // ============================================================

    double HFTAlgorithms::computeMomentum(const std::vector<Trade>& trades,
        size_t lookback)
    {
        if (trades.empty())
            return 0.0;

        size_t start =
            (trades.size() > lookback) ? trades.size() - lookback : 0;

        double firstPrice = trades[start].price;
        double lastPrice = trades.back().price;

        return lastPrice - firstPrice;
    }

    // ============================================================
    // ROLLING AVERAGE PRICE
    // ============================================================

    double HFTAlgorithms::computeRollingAverage(
        const std::vector<Trade>& trades,
        size_t lookback)
    {
        if (trades.empty())
            return 0.0;

        size_t start =
            (trades.size() > lookback) ? trades.size() - lookback : 0;

        double total = 0.0;
        uint64_t totalQty = 0;

        for (size_t i = start; i < trades.size(); ++i)
        {
            total += trades[i].price * trades[i].quantity;
            totalQty += trades[i].quantity;
        }

        if (totalQty == 0)
            return 0.0;

        return total / totalQty;
    }

    // ============================================================
    // PRINT ANALYTICS
    // ============================================================

    void HFTAlgorithms::printAnalytics(const OrderBook& book,
        const std::vector<Trade>& trades)
    {
        std::cout << "\n====== MARKET ANALYTICS ======\n";

        double imbalance = computeOrderImbalance(book);
        double spreadPct = computeSpreadPercentage(book);
        double momentum = computeMomentum(trades, 10);
        double rollingAvg = computeRollingAverage(trades, 10);

        std::cout << "Order Imbalance: " << imbalance << "\n";
        std::cout << "Spread %:        " << spreadPct << "%\n";
        std::cout << "Momentum:        " << momentum << "\n";
        std::cout << "Rolling Avg:     " << rollingAvg << "\n";
    }
}