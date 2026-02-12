#include "OrderBook.hpp"
#include "MatchingEngine.hpp"
#include "HFTUtils.hpp"
#include <iostream>

int main()
{
    OrderBook book;
    MatchingEngine engine(book);

    constexpr std::size_t TEST_ORDERS = 30000;

    HFT::runBenchmark([&]()
        {
            book.addOrder(
                { 1, OrderBook::Side::Buy, 100.0, 10 });

            book.addOrder(
                { 2, OrderBook::Side::Sell, 99.0, 10 });

            engine.match();

        }, TEST_ORDERS);

    book.printTop();

    auto latency = HFT::measureLatency([&]()
        {
            book.addOrder(
                { 3, OrderBook::Side::Buy, 101.0, 5 });
        });

    std::cout << "Single Order Latency: "
        << latency
        << " ns\n";

    return 0;

    std::cin.get();

}
