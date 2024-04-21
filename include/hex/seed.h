#pragma once

#include <cstddef>
#include <cstdint>
#include <random>

namespace hex
{
    template <typename Container>
    typename Container::value_type& any(Container& c)
    {
        return c.at(rand() % c.size());
    }

    template <typename G, typename T, std::size_t N>
    size_t multinomial(std::default_random_engine& generator, G& distribution, const std::array<T, N>& arr)
    {
        auto randomVal = distribution(generator);
        size_t rval{0};
        for(auto val : arr)
        {
            randomVal -= val;
            if(randomVal < 0)
            {
                break;
            }
            ++rval;
        }
        return rval;
    }

}