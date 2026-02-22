#pragma once

#include <chrono>
#include <cstdint>
#include <cmath>
#include <string>

namespace hft
{

    // ==============================
    // CONSTANTS
    // ==============================

    constexpr double EPSILON = 1e-9;
    constexpr size_t CACHE_LINE_SIZE = 64;

    // ==============================
    // NUMERICAL HELPERS
    // ==============================

    bool almostEqual(double a, double b, double eps = EPSILON);
    double normalizeToTick(double price, double tickSize);



    bool validateOrder(double price,
        uint64_t quantity,
        double maxPrice,
        uint64_t maxQty);



    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    TimePoint now();


    class LatencyTimer
    {
    public:
        void start();
        void stop();

        double elapsedNanoseconds() const;
        double elapsedMicroseconds() const;
        double elapsedMilliseconds() const;

    private:
        TimePoint startTime;
        TimePoint endTime;
    };

    // PNL

    double calculatePnL(double entryPrice,
        double exitPrice,
        uint64_t quantity,
        bool isLong);


    struct alignas(CACHE_LINE_SIZE) AlignedCounter
    {
        uint64_t value = 0;
    };

    class PerformanceCounter
    {
    public:
        void increment();
        uint64_t get() const;
        void reset();

    private:
        AlignedCounter counter;
    };

} // namespace hft