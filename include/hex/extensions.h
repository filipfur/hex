#pragma once

#include <functional>

namespace hex
{
    template <typename T, typename U>
    inline T as(U val)
    {
        return static_cast<T>(val);
    }

    template <typename T, typename U>
    inline T* cast(U* val)
    {
        return dynamic_cast<T*>(val);
    }

    template <typename T, typename U>
    inline bool contains(const T& container, const U& key)
    {
        auto it = container.find(key);
        return it != container.end();
    }

    template <typename T, typename U>
    inline size_t iterateAndRemove(std::unordered_map<T, U>& container, const std::function<bool(std::pair<T, U>&)>& func)
    {
        auto originalSize = container.size();
        auto newEnd = std::remove_if(container.begin(), container.end(), func);
        container.erase(newEnd, container.end());
        return originalSize - container.size();
    }

} // namespace hex