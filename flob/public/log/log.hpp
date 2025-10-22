#pragma once

#include "core_types.hpp"
#include "log/console.hpp"

#include <format>
#include <unordered_map>

namespace flob
{
    enum class LogLevel : uint8
    {
        Fatal,
        Error,
        Warn,
        Info,
        Debug,
        Trace
    };

    class Log final
    {
    public:
        Log() = delete;

    public:
        template <typename... Args>
        static auto fatal(std::format_string<Args...> fmt, Args&&... args) -> void;

        template <typename... Args>
        static auto error(std::format_string<Args...> fmt, Args&&... args) -> void;

        template <typename... Args>
        static auto warn(std::format_string<Args...> fmt, Args&&... args) -> void;

        template <typename... Args>
        static auto info(std::format_string<Args...> fmt, Args&&... args) -> void;

        template <typename... Args>
        static auto debug(std::format_string<Args...> fmt, Args&&... args) -> void;

        template <typename... Args>
        static auto trace(std::format_string<Args...> fmt, Args&&... args) -> void;

    private:
        struct LogInfo
        {
            ConsoleColor console_color;
            const char*  header;
            bool         is_error;
        };

        static const std::unordered_map<LogLevel, LogInfo> log_info_map;

    private:
        template <typename... Args>
        static auto log_message(LogLevel level, std::format_string<Args...> fmt, Args&&... args) -> void;
    };

    template <typename... Args>
    auto Log::fatal(std::format_string<Args...> fmt, Args&&... args) -> void
    {
        log_message(LogLevel::Fatal, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    auto Log::error(std::format_string<Args...> fmt, Args&&... args) -> void
    {
        log_message(LogLevel::Error, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    auto Log::warn(std::format_string<Args...> fmt, Args&&... args) -> void
    {
        log_message(LogLevel::Warn, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    auto Log::info(std::format_string<Args...> fmt, Args&&... args) -> void
    {
        log_message(LogLevel::Info, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    auto Log::debug(std::format_string<Args...> fmt, Args&&... args) -> void
    {
        log_message(LogLevel::Debug, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    auto Log::trace(std::format_string<Args...> fmt, Args&&... args) -> void
    {
        log_message(LogLevel::Trace, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    auto Log::log_message(LogLevel level, std::format_string<Args...> fmt, Args&&... args) -> void
    {
        const auto [console_color, header, is_error] = log_info_map.at(level);
        const auto message                           = std::format(fmt, std::forward<Args>(args)...);
        if (level == LogLevel::Info || level == LogLevel::Warn)
        {
            const auto full_message = std::format("[{}]:  {}", header, message);
            Console::write_line(full_message.c_str(), is_error, console_color);
        }
        else
        {
            const auto full_message = std::format("[{}]: {}", header, message);
            Console::write_line(full_message.c_str(), is_error, console_color);
        }
    }
}
