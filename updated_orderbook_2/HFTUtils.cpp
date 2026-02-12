#include "HFTUtils.hpp"
#include <iostream>

namespace HFT
{

    std::uint64_t measureLatency(
        const std::function<void()>& func) noexcept
    {
        auto start =
            std::chrono::high_resolution_clock::now();

        func();

        auto end =
            std::chrono::high_resolution_clock::now();

        return std::chrono::duration_cast<
            std::chrono::nanoseconds>(end - start).count();
    }

    void runBenchmark(
        const std::function<void()>& func,
        std::size_t iterations) noexcept
    {
        auto start =
            std::chrono::high_resolution_clock::now();

        for (std::size_t i = 0; i < iterations; ++i)
        {
            if ( i < iterations)
                func();
        }

        auto end =
            std::chrono::high_resolution_clock::now();

        auto duration =
            std::chrono::duration_cast<
            std::chrono::microseconds>(end - start).count();

        std::cout << "Benchmark Time: "
            << duration
            << " microseconds\n";
    }

}
