#pragma once

#include <map>
#include <deque>
#include <cstdint>
#include <chrono>
#include <optional>
#include <compare>
#include <functional>

/*
    OrderBook
    Core data structure for price-time FIFO limit book.
*/

class OrderBook {
public:

    using OrderId = std::uint64_t;
    using Quantity = std::uint64_t;
    using Price = double;

    enum class Side {
        Buy,
        Sell
    };

    struct Order {
        OrderId id{};
        Side side{};
        Price price{};
        Quantity quantity{};
        std::chrono::steady_clock::time_point timestamp;

        Order(OrderId i,
            Side s,
            Price p,
            Quantity q) noexcept
            : id(i), side(s), price(p), quantity(q),
            timestamp(std::chrono::steady_clock::now()) {
        }

        // C++20 spaceship
        auto operator<=>(const Order&) const = default;
    };

private:

    using PriceLevel = std::deque<Order>;

    std::map<Price, PriceLevel, std::greater<>> bids;
    std::map<Price, PriceLevel, std::less<>> asks;

public:

    void addOrder(Order order);

    auto& getBids() noexcept { return bids; }
    auto& getAsks() noexcept { return asks; }

    [[nodiscard]]
    std::optional<Order> bestBid() const noexcept;

    [[nodiscard]]
    std::optional<Order> bestAsk() const noexcept;

    void printTop() const noexcept;
};
