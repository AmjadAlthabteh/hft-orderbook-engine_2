#pragma once

#include <map>
#include <deque>
#include <vector>
#include <cstdint>
#include <chrono>
#include <functional>
#include <optional>
#include <iostream>

/*
    ==========================================================
                    HIGH FREQUENCY ORDER BOOK
    ==========================================================

    Realistic limit order book implementing:

    ? Separate bid / ask books
    ? Price-time priority (FIFO per price level)
    ? Partial fills
    ? Volume aggregation
    ? Spread & mid-price
    ? VWAP calculation
    ? Market depth printing
    ? Clean extensible architecture

    Design Philosophy:
    - Keep matching logic centralized
    - Avoid redundant recalculations
    - Keep price levels cache-friendly
    - Maintain exchange-like behavior
*/

namespace hft
{

    // ============================================================
    // ENUMS
    // ============================================================

    enum class Side
    {
        Buy,
        Sell
    };

    enum class OrderType
    {
        Market,
        Limit
    };

    // ============================================================
    // ORDER STRUCT
    // ============================================================

    struct Order
    {
        uint64_t id;
        Side side;
        OrderType type;
        double price;
        uint64_t quantity;
        uint64_t originalQty;
        std::chrono::steady_clock::time_point timestamp;

        Order(uint64_t id_,
            Side side_,
            OrderType type_,
            double price_,
            uint64_t qty_)
            : id(id_),
            side(side_),
            type(type_),
            price(price_),
            quantity(qty_),
            originalQty(qty_),
            timestamp(std::chrono::steady_clock::now())
        {
        }
    };

    // ============================================================
    // TRADE STRUCT
    // ============================================================

    struct Trade
    {
        uint64_t buyOrderId;
        uint64_t sellOrderId;
        double price;
        uint64_t quantity;
        std::chrono::steady_clock::time_point timestamp;
    };

    // ============================================================
    // PRICE LEVEL
    // ============================================================

    struct PriceLevel
    {
        std::deque<Order> orders;
        uint64_t totalVolume = 0;

        void addOrder(const Order& order)
        {
            orders.push_back(order);
            totalVolume += order.quantity;
        }

        void reduceFront(uint64_t qty)
        {
            if (orders.empty())
                return;

            Order& front = orders.front();
            front.quantity -= qty;
            totalVolume -= qty;

            if (front.quantity == 0)
                orders.pop_front();
        }

        bool empty() const
        {
            return orders.empty();
        }
    };

    // ============================================================
    // ORDER BOOK
    // ============================================================

    class OrderBook
    {
    public:

        OrderBook() = default;

        // Order entry
        void addOrder(const Order& order);

        // Matching engine trigger
        std::vector<Trade> match();

        // Market data
        double getBestBid() const;
        double getBestAsk() const;
        double getSpread() const;
        double getMidPrice() const;

        uint64_t getTotalBidVolume() const;
        uint64_t getTotalAskVolume() const;

        double calculateVWAP() const;

        // Output
        void printTopOfBook() const;
        void printFullDepth() const;

        // Utilities
        bool empty() const;
        void clear();

    private:

        // Descending bids (highest first)
        std::map<double, PriceLevel, std::greater<double>> bids;

        // Ascending asks (lowest first)
        std::map<double, PriceLevel> asks;

        std::vector<Trade> trades;

        void matchOrders();
    };

} // namespace hft