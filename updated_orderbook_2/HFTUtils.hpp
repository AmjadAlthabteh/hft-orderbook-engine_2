#pragma once
#include <chrono>
#include <functional>
#include <cstdint>

namespace HFT
{
    struct alignas(64) CacheAlignedCounter
    {
        std::uint64_t value{ 0 };
    };

    [[nodiscard]]
    std::uint64_t measureLatency(
        const std::function<void()>& func) noexcept;

    void runBenchmark(
        const std::function<void()>& func,
        std::size_t iterations) noexcept;
}
