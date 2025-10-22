#pragma once

#include "core_types.hpp"

namespace flob
{
    enum class Side : uint8
    {
        Buy,
        Sell
    };

    enum class OrderType : uint8
    {
        None,
        GTC,  // Good Till Cancel
        IOC,  // Immediate Or Cancel
        GFD,  // Good For Day
        FOK,  // Fill Or Kill
    };
}
