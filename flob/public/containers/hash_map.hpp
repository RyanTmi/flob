#pragma once

#include <unordered_map>

namespace flob
{
    // TODO: Implement own HashMap.
    template <typename K, typename T, typename Hash = std::hash<K>>
    using HashMap = std::unordered_map<K, T, Hash>;
}
