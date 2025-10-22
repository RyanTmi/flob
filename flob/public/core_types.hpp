#pragma once

namespace flob
{
    using uint8  = unsigned char;
    using uint16 = unsigned short;
    using uint32 = unsigned int;
    using uint64 = unsigned long long;

    using int8  = signed char;
    using int16 = signed short;
    using int32 = signed int;
    using int64 = signed long long;

    using float32 = float;
    using float64 = double;

    using usize = uint64;
    using ssize = int64;

    static_assert(sizeof(uint8) == 1, "Size of uint8 should be 1 byte");
    static_assert(sizeof(uint16) == 2, "Size of uint16 should be 2 bytes");
    static_assert(sizeof(uint32) == 4, "Size of uint32 should be 4 bytes");
    static_assert(sizeof(uint64) == 8, "Size of uint64 should be 8 bytes");

    static_assert(sizeof(int8) == 1, "Size of int8 should be 1 byte");
    static_assert(sizeof(int16) == 2, "Size of int16 should be 2 bytes");
    static_assert(sizeof(int32) == 4, "Size of int32 should be 4 bytes");
    static_assert(sizeof(int64) == 8, "Size of int64 should be 8 bytes");

    static_assert(sizeof(float32) == 4, "Size of float32 should be 4 bytes");
    static_assert(sizeof(float64) == 8, "Size of float64 should be 8 bytes");
}
