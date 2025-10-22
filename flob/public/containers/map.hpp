#pragma once

#include <map>

namespace flob
{
    // TODO: Implement own Map.
    template <typename K, typename T, typename Compare = std::less<K>>
    using Map = std::map<K, T, Compare>;
}
