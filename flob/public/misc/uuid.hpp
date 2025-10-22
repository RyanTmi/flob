#pragma once

#include "core_types.hpp"

#include <cstddef>
#include <functional>

namespace flob
{
    class UUID final
    {
    public:
        UUID();
        UUID(uint64 uuid);
        UUID(const UUID&) = default;

        constexpr operator uint64() const { return _uuid; }

    private:
        uint64 _uuid;
    };
}

namespace std
{
    template <>
    struct hash<flob::UUID>
    {
        std::size_t operator()(const flob::UUID& id) const noexcept { return std::hash<flob::uint64> {}(static_cast<flob::uint64>(id)); }
    };
}
