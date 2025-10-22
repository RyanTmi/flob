#include "log/log.hpp"

namespace flob
{
    const std::unordered_map<LogLevel, Log::LogInfo> Log::log_info_map = {
        {LogLevel::Fatal, {ConsoleColor::Red_Background, "FATAL", true}},
        {LogLevel::Error, {ConsoleColor::Red, "ERROR", true}           },
        {LogLevel::Warn,  {ConsoleColor::Yellow, "WARN", false}        },
        {LogLevel::Info,  {ConsoleColor::Green, "INFO", false}         },
        {LogLevel::Debug, {ConsoleColor::Blue, "DEBUG", false}         },
        {LogLevel::Trace, {ConsoleColor::Default, "TRACE", false}      }
    };
}
