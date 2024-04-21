#pragma once

#include <iostream>

namespace hex
{
#ifdef CYCLES_ENABLE
    static inline uint64_t get_cycles()
    {
        uint64_t t;
        __asm volatile ("rdtsc" : "=A"(t));
        return t;
    }
 
    template <size_t N>
    struct Measurement
    {
        uint64_t stamp{0};
        uint32_t index{0};
        std::array<uint64_t, N> points;
    };
 
    constexpr std::string_view file_name(const std::string_view& str)
    {
        auto i = str.rfind('/') + 1;
        return str.substr(i, str.length() - i);
    }
 
    #define CYCLES_START(var, N) static hex::Measurement<N> var; var.stamp = hex::get_cycles();
    #define CYCLES_END(var) var.points[var.index == var.points.size() ? (var.index=0)++ : var.index++] = hex::get_cycles() - var.stamp;
    #define CYCLES_ALL(var) if(var.index == var.points.size()) { constexpr std::string_view fileName = hex::file_name(__FILE__); printf("CYCLES_ALL(%s %s:%d): ", #var, fileName.data(), __LINE__); std::copy(var.points.begin(), var.points.begin() + var.index, std::ostream_iterator<uint64_t>(std::cout, " ")); std::cout << std::endl; }
    #define CYCLES_AVG(var) if(var.index == var.points.size()) { constexpr std::string_view fileName = hex::file_name(__FILE__); printf("CYCLES_AVG(%s %s:%d): %d\n", #var, fileName.data(), __LINE__, std::accumulate(var.points.begin(), var.points.begin() + var.index, 1) / var.index); }
#else
    #define CYCLES_START(var, N)
    #define CYCLES_END(var)
    #define CYCLES_ALL(var)
    #define CYCLES_AVG(var)
#endif
} // namespace hex