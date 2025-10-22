#include "order_book/order_book.hpp"

#include "log/log.hpp"

#include <numeric>

namespace flob
{
    OrderBook::OrderBook(const OrderBookConfig& config)
        : _session(config.session)
        , _gfd_expired_today(false)
    {}

    auto OrderBook::infos() const -> OrderBookInfos
    {
        OrderBookInfos infos;
        infos.bids.reserve(_bids.size());
        infos.asks.reserve(_asks.size());

        auto create_level_infos = [](auto price, const auto& orders) {
            auto quantity = std::accumulate(orders.begin(), orders.end(), static_cast<Quantity>(0), [](auto s, const auto& o) {
                return s + o->remaining_quantity();
            });
            return OrderBookLevelInfos(price, quantity);
        };

        for (const auto& [price, orders] : _bids)
        {
            infos.bids.push_back(create_level_infos(price, orders));
        }
        for (const auto& [price, orders] : _asks)
        {
            infos.asks.push_back(create_level_infos(price, orders));
        }

        return infos;
    }

    auto OrderBook::add_order(const OrderRef& order) -> Vector<Trade>
    {
        cancel_gfd_if_needed();

        if (_orders.contains(order->id()))
        {
            Log::warn("Order with ID {} already exists in order book.", static_cast<uint64>(order->id()));
            return {};
        }

        // TODO: Handle market order first

        OrderRefs::iterator location;
        switch (order->side())
        {
            case Side::Sell:
            {
                auto& orders = _asks[order->price()];
                orders.push_back(order);
                location = std::prev(orders.end());
                break;
            }
            case Side::Buy:
            {
                auto& orders = _bids[order->price()];
                orders.push_back(order);
                location = std::prev(orders.end());
                break;
            }
            default: Log::error("Unknown order side."); return {};
        }

        _orders.emplace(order->id(), OrderEntry(order, location));
        return match_orders();
    }

    auto OrderBook::cancel_order(OrderId order_id) -> void
    {
        if (!_orders.contains(order_id))
        {
            Log::warn("Order with ID {} does not exist in order book.", static_cast<uint64>(order_id));
            return;
        }

        const auto& [order, location] = _orders.at(order_id);
        const auto price              = order->price();
        switch (order->side())
        {
            case Side::Sell:
            {
                auto& orders = _asks.at(price);
                orders.erase(location);
                if (orders.empty())
                {
                    _asks.erase(price);
                }
                break;
            }
            case Side::Buy:
            {
                auto& orders = _bids.at(price);
                orders.erase(location);
                if (orders.empty())
                {
                    _bids.erase(price);
                }
                break;
            }
            default: Log::error("Unknown order side."); return;
        }

        _orders.erase(order_id);
    }

    auto OrderBook::cancel_orders(OrderType order_type) -> void
    {
        for (auto it = _orders.begin(); it != _orders.end();)
        {
            const auto& [order_id, entry] = *it;
            const auto& [order, location] = entry;

            if (order->type() != order_type)
            {
                ++it;
                continue;
            }

            const auto price = order->price();
            switch (order->side())
            {
                case Side::Sell:
                {
                    auto& orders = _asks.at(price);
                    orders.erase(location);
                    if (orders.empty())
                    {
                        _asks.erase(price);
                    }
                    break;
                }
                case Side::Buy:
                {
                    auto& orders = _bids.at(price);
                    orders.erase(location);
                    if (orders.empty())
                    {
                        _bids.erase(price);
                    }
                    break;
                }
                default: break;
            }

            it = _orders.erase(it);
        }
    }

    auto OrderBook::match_orders() -> Vector<Trade>
    {
        Vector<Trade> trades;
        trades.reserve(_orders.size());

        while (true)
        {
            if (_bids.empty() || _asks.empty())
            {
                // No more matches possible.
                break;
            }

            auto& [bid_price, bid_orders] = *_bids.begin();
            auto& [ask_price, ask_orders] = *_asks.begin();

            if (bid_price < ask_price)
            {
                // No overlap in prices, can't match.
                break;
            }

            Vector<OrderId> filled_orders;
            while (!bid_orders.empty() && !ask_orders.empty())
            {
                const auto& bid = bid_orders.front();
                const auto& ask = ask_orders.front();

                const auto quantity = std::min(bid->remaining_quantity(), ask->remaining_quantity());

                // Record the trade before modifying orders
                trades.emplace_back(bid->id(), ask->id(), bid->price(), ask->price(), quantity);

                bid->fill(quantity);
                ask->fill(quantity);
                if (bid->is_filled())
                {
                    _orders.erase(bid->id());
                    bid_orders.pop_front();
                }
                if (ask->is_filled())
                {
                    _orders.erase(ask->id());
                    ask_orders.pop_front();
                }
            }

            if (bid_orders.empty())
            {
                _bids.erase(bid_price);
            }
            if (ask_orders.empty())
            {
                _asks.erase(ask_price);
            }
        }

        return trades;
    }

    auto OrderBook::cancel_gfd_if_needed() -> void
    {
        // When session is closed, expire all GFD orders once
        if (!_gfd_expired_today && _session.is_close())
        {
            cancel_orders(OrderType::GFD);
            _gfd_expired_today = true;
            Log::trace("Expired all GFD orders at market close");
        }

        // When session reopens, reset daily expiry flag
        if (_gfd_expired_today && _session.is_open())
        {
            _gfd_expired_today = false;
            Log::trace("Reset GFD expiry state for new session");
        }
    }
}
