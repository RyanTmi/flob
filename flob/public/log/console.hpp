#pragma once

#include "core_types.hpp"

namespace flob
{
    enum class ConsoleColor : uint8
    {
        Default,

        Black,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White,

        Black_Background,
        Red_Background,
        Green_Background,
        Yellow_Background,
        Blue_Background,
        Magenta_Background,
        Cyan_Background,
        White_Background,
    };

    class Console final
    {
    public:
        Console() = delete;

    public:
        static auto write_line(const char* message, bool is_error = false, ConsoleColor console_color = ConsoleColor::Default) -> void;
    };
}
