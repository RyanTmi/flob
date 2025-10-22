#pragma once

#include "log/log.hpp"

#include <source_location>
#include <string_view>

#if defined(NDEBUG)
    #define DEBUG_BREAK()
#else
    #if defined(_MSC_VER)
        #define DEBUG_BREAK() __debugbreak()
    #elif defined(__clang__) || defined(__GNUC__)
        #include <csignal>
        #define DEBUG_BREAK() std::raise(SIGTRAP)
    #else
        #include <stdlib.h>
        #define DEBUG_BREAK() abort()
    #endif
#endif

namespace flob
{
#if defined(NDEBUG)
    constexpr auto ensure([[maybe_unused]] bool condition, [[maybe_unused]] std::string_view message = {}) -> void {}
#else
    constexpr auto ensure(bool condition, std::string_view message = {}, const std::source_location& loc = std::source_location::current()) -> void
    {
        if (!condition)
        {
            Log::error("{}:{} in {}\nCondition failed", loc.file_name(), loc.line(), loc.function_name());
            if (!message.empty())
            {
                Log::error("{}", message);
            }
            DEBUG_BREAK();
        }
    }
#endif
}
