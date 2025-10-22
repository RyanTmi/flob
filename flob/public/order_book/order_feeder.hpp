#pragma once

#include "memory/ref.hpp"
#include "order_book/order.hpp"
#include "order_book/order_type.hpp"
#include "order_book/types.hpp"

#include <random>

namespace flob
{
    struct OrderFlowConfig
    {
        Price    mid_price = 10'000;  // e.g. $100.00 in cents
        int32    max_drift = 250;     // +/- ticks around mid
        Quantity min_qty   = 1;
        Quantity max_qty   = 100;
    };

    class OrderFeeder
    {
    public:
        explicit OrderFeeder(const OrderFlowConfig& cfg = {})
            : _cfg(cfg)
            , _rng(std::random_device {}())
            , _side(0, 1)
            , _drift(-cfg.max_drift, cfg.max_drift)
            , _qty(cfg.min_qty, cfg.max_qty)
        {}

        auto next() -> Ref<Order>
        {
            Side side = _side(_rng) == 0 ? Side::Buy : Side::Sell;

            Price price = _cfg.mid_price + _drift(_rng);
            price       = price <= 0 ? 1 : price;

            Quantity qty = _qty(_rng);

            return make_ref<Order>(OrderType::GTC, side, price, qty);
        }

    private:
        OrderFlowConfig                       _cfg;
        std::mt19937_64                       _rng;
        std::uniform_int_distribution<uint32> _side;
        std::uniform_int_distribution<int32>  _drift;
        std::uniform_int_distribution<uint32> _qty;
    };
}
