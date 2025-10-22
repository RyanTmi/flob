#pragma once

#include <chrono>

namespace flob
{
    struct Session
    {
        using Clock     = std::chrono::system_clock;
        using TimePoint = std::chrono::time_point<Clock>;
        using Minutes   = std::chrono::minutes;

        std::chrono::hh_mm_ss<Minutes> open;
        std::chrono::hh_mm_ss<Minutes> close;

        [[nodiscard]] auto is_open(TimePoint now = Clock::now()) const noexcept -> bool;
        [[nodiscard]] auto is_close(TimePoint now = Clock::now()) const noexcept -> bool;
    };

    constexpr Session new_york_session = {
        .open  = std::chrono::hh_mm_ss(Session::Minutes(9 * 60 + 30)),  // 09:30
        .close = std::chrono::hh_mm_ss(Session::Minutes(16 * 60)),      // 16:00
    };

    //==============================================================================================
    // struct : Session
    //==============================================================================================

    inline auto Session::is_open(TimePoint now) const noexcept -> bool
    {
        using namespace std::chrono;

        const auto time = floor<minutes>(now.time_since_epoch());
        const auto day  = floor<days>(time);
        const auto tod  = time - day;

        const auto open_time  = open.hours() + open.minutes();
        const auto close_time = close.hours() + close.minutes();

        return tod >= open_time && tod < close_time;
    }

    inline auto Session::is_close(TimePoint now) const noexcept -> bool
    {
        using namespace std::chrono;

        const auto time = floor<minutes>(now.time_since_epoch());
        const auto day  = floor<days>(time);
        const auto tod  = time - day;

        const auto open_time  = open.hours() + open.minutes();
        const auto close_time = close.hours() + close.minutes();

        return tod < open_time || tod >= close_time;
    }
}
