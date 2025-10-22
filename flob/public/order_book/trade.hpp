#pragma once

#include "order_book/types.hpp"

#include <format>

namespace flob
{
    struct Trade
    {
        OrderId  bid_id;
        OrderId  ask_id;
        Price    bid_price;
        Price    ask_price;
        Quantity quantity;
    };
}

template <>
struct std::formatter<flob::Trade>
{
    constexpr auto parse(std::format_parse_context& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const flob::Trade& trade, FormatContext& ctx) const
    {
        auto out = ctx.out();

        out = std::format_to(out, "{{Bid ID: {}, ", static_cast<flob::uint64>(trade.bid_id));
        out = std::format_to(out, "Ask ID: {}, ", static_cast<flob::uint64>(trade.ask_id));
        out = std::format_to(out, "Bid Price: {}, ", trade.bid_price);
        out = std::format_to(out, "Ask Price: {}, ", trade.ask_price);
        out = std::format_to(out, "Quantity: {}}}", trade.quantity);
        return out;
    }
};
