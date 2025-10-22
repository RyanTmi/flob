#pragma once

#include "containers/hash_map.hpp"
#include "containers/map.hpp"
#include "containers/vector.hpp"
#include "memory/ref.hpp"
#include "order_book/order.hpp"
#include "order_book/session.hpp"
#include "order_book/trade.hpp"

#include <deque>

namespace flob
{
    using OrderRef  = Ref<Order>;
    using OrderRefs = std::deque<OrderRef>;

    struct OrderBookLevelInfos
    {
        Price    price;
        Quantity quantity;
    };

    struct OrderBookInfos
    {
        Vector<OrderBookLevelInfos> bids;
        Vector<OrderBookLevelInfos> asks;
    };

    struct OrderBookConfig
    {
        Session session;
    };

    class OrderBook
    {
    public:
        explicit OrderBook(const OrderBookConfig& config);

    public:
        [[nodiscard]] constexpr auto size() const noexcept -> usize;

        [[nodiscard]] auto infos() const -> OrderBookInfos;

        auto add_order(const OrderRef& order) -> Vector<Trade>;

    private:
        auto cancel_order(OrderId order_id) -> void;
        auto cancel_orders(OrderType order_type) -> void;

        auto match_orders() -> Vector<Trade>;

        auto cancel_gfd_if_needed() -> void;

        struct OrderEntry
        {
            OrderRef            order;
            OrderRefs::iterator location;
        };

    private:
        Map<Price, OrderRefs, std::greater<Price>> _bids;
        Map<Price, OrderRefs, std::less<Price>>    _asks;
        HashMap<OrderId, OrderEntry>               _orders;

        Session _session;
        bool    _gfd_expired_today;
    };

    //==============================================================================================
    // class : OrderBook
    //==============================================================================================

    constexpr auto OrderBook::size() const noexcept -> usize
    {
        return _orders.size();
    }
}
