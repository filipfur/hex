#pragma once

namespace hex
{
    template <typename Container>
    typename Container::value_type& any(Container& c)
    {
        return c.at(rand() % c.size());
    }
}