#include "OrderBook.hpp"
#include <iostream>

void OrderBook::addOrder(Order order)
{
    if (order.side == Side::Buy)
        bids[order.price].push_back(std::move(order));
    else
        asks[order.price].push_back(std::move(order));
}

std::optional<OrderBook::Order>
OrderBook::bestBid() const noexcept
{
    if (bids.empty())
        return std::nullopt;

    return bids.begin()->second.front();
}

std::optional<OrderBook::Order>
OrderBook::bestAsk() const noexcept
{
    if (asks.empty())
        return std::nullopt;

    return asks.begin()->second.front();
}

void OrderBook::printTop() const noexcept
{
    if (auto bid = bestBid(); bid)
        std::cout << "Best Bid: "
        << bid->price
        << " Qty: "
        << bid->quantity << "\n";

    if (auto ask = bestAsk(); ask)
        std::cout << "Best Ask: "
        << ask->price
        << " Qty: "
        << ask->quantity << "\n";
}
