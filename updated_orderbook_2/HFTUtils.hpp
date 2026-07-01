#pragma once

#include <chrono>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>

/*


    Purpose:
        Performance tools
        Time measurement
        Numerical safety
        Risk checks
        Microstructure helpers
        Low-level performance utilities

    Designed for:
        - Trading engines
        - Backtesting systems
        - Latency profiling
        - Quant infrastructure
*/

namespace hft
{



    constexpr double EPSILON = 1e-9;
    constexpr size_t CACHE_LINE_SIZE = 64;



    [[nodiscard]] inline bool almostEqual(double a, double b, double eps = EPSILON)
    {
        return std::fabs(a - b) < eps;
    }


    [[nodiscard]] inline double normalizeToTick(double price, double tickSize)
    {
        return std::round(price / tickSize) * tickSize;
    }


    [[nodiscard]] inline bool validateOrder(double price,
        uint64_t quantity,
        double maxPrice,
        uint64_t maxQty)
    {
        if (!std::isfinite(price) || !std::isfinite(maxPrice))
            return false;

        if (price <= 0.0 || quantity == 0)
            return false;

        if (price > maxPrice || quantity > maxQty)
            return false;

        return true;
    }


    [[nodiscard]] inline double computeVWAP(double totalValue,
        uint64_t totalVolume)
    {
        if (totalVolume == 0)
            return 0.0;

        return totalValue / totalVolume;
    }

    // ============================================================
    // HIGH RESOLUTION CLOCK
    // ============================================================

    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    [[nodiscard]] inline TimePoint now()
    {
        return Clock::now();
    }

    template <typename Func>
    [[nodiscard]]
    inline uint64_t measureLatency(Func&& func) noexcept
    {
        const auto start = Clock::now();

        func();

        const auto end = Clock::now();

        return std::chrono::duration_cast<
            std::chrono::nanoseconds>(end - start).count();
    }

    template <typename Func>
    [[nodiscard]] inline uint64_t runBenchmark(Func&& func, size_t iterations) noexcept
    {
        const auto start = Clock::now();

        for (size_t i = 0; i < iterations; ++i)
            func();

        const auto end = Clock::now();

        return std::chrono::duration_cast<
            std::chrono::microseconds>(end - start).count();
    }

    // ============================================================
    // LATENCY TIMER CLASS
    // ============================================================

    class LatencyTimer
    {
    public:

        void start()
        {
            startTime = Clock::now();
        }

        void stop()
        {
            endTime = Clock::now();
        }

        [[nodiscard]] double elapsedNanoseconds() const
        {
            return std::chrono::duration<double, std::nano>(endTime - startTime).count();
        }

        [[nodiscard]] double elapsedMicroseconds() const
        {
            return std::chrono::duration<double, std::micro>(endTime - startTime).count();
        }

        [[nodiscard]] double elapsedMilliseconds() const
        {
            return std::chrono::duration<double, std::milli>(endTime - startTime).count();
        }

    private:
        TimePoint startTime;
        TimePoint endTime;
    };

    // ============================================================
    // PNL CALCULATOR
    // ============================================================

    [[nodiscard]] inline double calculatePnL(double entryPrice,
        double exitPrice,
        uint64_t quantity,
        bool isLong)
    {
        if (isLong)
            return (exitPrice - entryPrice) * quantity;
        else
            return (entryPrice - exitPrice) * quantity;
    }


    struct alignas(CACHE_LINE_SIZE) AlignedCounter
    {
        uint64_t value = 0;
    };


    class PerformanceCounter
    {
    public:

        void increment()
        {
            ++counter.value;
        }

        [[nodiscard]] uint64_t get() const
        {
            return counter.value;
        }

        void reset()
        {
            counter.value = 0;
        }

    private:
        AlignedCounter counter;
    };


    inline void printSeparator()
    {
        std::cout << "----------------------------------------\n";
    }

    inline void printTimestamp()
    {
        // Get current system time point
        auto now = std::chrono::system_clock::now();

        // Convert to time_t
        std::time_t time = std::chrono::system_clock::to_time_t(now);

        // Thread-safe localtime (MSVC safe)
        std::tm tmStruct{};
        localtime_s(&tmStruct, &time);

        std::cout << std::put_time(&tmStruct, "%H:%M:%S");
    }

} // namespace hft
