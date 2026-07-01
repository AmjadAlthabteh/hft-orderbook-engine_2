#include "HFTAlgorithms.hpp"
#include "MatchingEngine.hpp"

#include <cassert>

using namespace hft;

namespace
{
    void limitOrdersMatchAtRestingAsk()
    {
        MatchingEngine engine;

        const auto sellId = engine.submitOrder(Side::Sell, OrderType::Limit, 101.0, 500);
        const auto buyId = engine.submitOrder(Side::Buy, OrderType::Limit, 101.0, 200);

        const auto& trades = engine.getTrades();
        assert(trades.size() == 1);
        assert(trades.front().buyOrderId == buyId);
        assert(trades.front().sellOrderId == sellId);
        assert(trades.front().price == 101.0);
        assert(trades.front().quantity == 200);
        assert(engine.getOrderBook().getTotalAskVolume() == 300);
        assert(engine.getOrderBook().getTotalBidVolume() == 0);
    }

    void marketOrdersConsumeBestPriceFirst()
    {
        MatchingEngine engine;

        const auto lowAskId = engine.submitOrder(Side::Sell, OrderType::Limit, 100.0, 100);
        engine.submitOrder(Side::Sell, OrderType::Limit, 101.0, 100);
        const auto marketBuyId = engine.submitOrder(Side::Buy, OrderType::Market, 0.0, 50);

        const auto& trades = engine.getTrades();
        assert(trades.size() == 1);
        assert(trades.front().buyOrderId == marketBuyId);
        assert(trades.front().sellOrderId == lowAskId);
        assert(trades.front().price == 100.0);
        assert(trades.front().quantity == 50);
        assert(engine.getOrderBook().getTotalAskVolume() == 150);
    }

    void riskLimitsRejectInvalidOrders()
    {
        MatchingEngine engine;
        engine.setRiskLimits({ 1'000.0, 1'000, false });

        assert(engine.submitOrder(Side::Buy, OrderType::Limit, 1'001.0, 1) == 0);
        assert(engine.submitOrder(Side::Buy, OrderType::Limit, 100.0, 1'001) == 0);
        assert(engine.submitOrder(Side::Buy, OrderType::Market, 0.0, 1) == 0);
        assert(engine.getTrades().empty());
        assert(engine.getOrderBook().empty());
    }
}

int main()
{
    limitOrdersMatchAtRestingAsk();
    marketOrdersConsumeBestPriceFirst();
    riskLimitsRejectInvalidOrders();

    return 0;
}
