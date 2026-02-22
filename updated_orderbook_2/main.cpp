#include "MatchingEngine.hpp"
#include "HFTAlgorithms.hpp"
#include "HFTUtils.hpp"

#include <iostream>
#include <thread>
#include <random>

using namespace hft;

/*
    This simulates:

        ✔ Order submissions
        ✔ Bid / Ask crossing
        ✔ Trade execution
        ✔ Market depth
        ✔ Analytics
        ✔ Latency measurement

    Compile example:
        g++ -std=c++20 *.cpp -O2 -o engine
*/

int main()
{
    std::cout << "====================================\n";
    std::cout << "      HFT MATCHING ENGINE DEMO\n";
    std::cout << "====================================\n";

    MatchingEngine engine;
    LatencyTimer timer;

    // Start latency measurement
    timer.start();


    engine.submitOrder(Side::Sell, OrderType::Limit, 101.0, 500);
    engine.submitOrder(Side::Sell, OrderType::Limit, 102.0, 300);
    engine.submitOrder(Side::Sell, OrderType::Limit, 103.0, 200);

    engine.submitOrder(Side::Buy, OrderType::Limit, 99.0, 400);
    engine.submitOrder(Side::Buy, OrderType::Limit, 98.5, 250);
    engine.submitOrder(Side::Buy, OrderType::Limit, 97.0, 100);


    engine.submitOrder(Side::Buy, OrderType::Limit, 101.0, 600);
    engine.submitOrder(Side::Sell, OrderType::Limit, 99.0, 350);

    timer.stop();


    engine.printFullDepth();
    engine.printTopOfBook();

    const auto& trades = engine.getTrades();

    std::cout << "\n====== TRADES EXECUTED ======\n";
    for (const auto& t : trades)
    {
        std::cout << "BuyID: " << t.buyOrderId
            << " | SellID: " << t.sellOrderId
            << " | Price: " << t.price
            << " | Qty: " << t.quantity
            << "\n";
    }


    std::cout << "\n";
    HFTAlgorithms::printAnalytics(engine.getTrades().empty()
        ? OrderBook()
        : *(new OrderBook()), // placeholder safety
        trades);


    std::cout << "\n====== PERFORMANCE ======\n";
    std::cout << "Execution Time (microseconds): "
        << timer.elapsedMicroseconds() << "\n";

    std::cout << "Execution Time (nanoseconds):  "
        << timer.elapsedNanoseconds() << "\n";

    std::cout << "\nSimulation Complete.\n";

    return 0;
}
