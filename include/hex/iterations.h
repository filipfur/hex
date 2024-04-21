#pragma once

#include <cstdint>
#include <functional>
#include <numeric>

namespace hex
{
    template <typename Container>
    size_t iter_remove_if(Container& c, const std::function<bool(typename Container::value_type&)>& func)
    {
        auto size = c.size();
        auto it = std::begin(c);
        while(it != std::end(c))
        {
            if(func(*it))
            {
                it = c.erase(it);
            }
            else
            {
                ++it;
            }
        }
        return size - c.size();
    }

    template <typename Container>
    void iter_combine(Container& c, const std::function<void(typename Container::value_type&, typename Container::value_type)>& func)
    {
        for(auto it = std::begin(c); it != std::end(c); ++it)
        {
            for(auto next = std::next(it); next != std::end(c); ++next)
            {
                func(*it, *next);
            }
        }
    }

    template <typename Container>
    typename Container::value_type iter_sum(const Container& c)
    {
        static constexpr typename Container::value_type init{};
        return std::accumulate(std::begin(c), std::end(c), init);
    }
}