#pragma once

#include "debug/ensure.hpp"
#include "memory/ref_counted.hpp"
#include "order_book/order_type.hpp"
#include "order_book/types.hpp"

#include <chrono>
#include <limits>

namespace flob
{
    constexpr auto invalid_price = std::numeric_limits<Price>::max();

    using Clock     = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    class Order : public RefCounted<Order>
    {
    public:
        Order(OrderType type, Side side, Price price, Quantity quantity) noexcept;
        Order(Side side, Quantity quantity) noexcept;

        ~Order() noexcept = default;

    public:
        [[nodiscard]] constexpr auto id() const noexcept -> OrderId;
        [[nodiscard]] constexpr auto time_point() const noexcept -> TimePoint;
        [[nodiscard]] constexpr auto price() const noexcept -> Price;
        [[nodiscard]] constexpr auto remaining_quantity() const noexcept -> Quantity;
        [[nodiscard]] constexpr auto type() const noexcept -> OrderType;
        [[nodiscard]] constexpr auto side() const noexcept -> Side;

        [[nodiscard]] constexpr auto is_market_order() const noexcept -> bool;

        constexpr auto               fill(Quantity quantity) noexcept -> void;
        [[nodiscard]] constexpr auto is_filled() const noexcept -> bool;

    private:
        OrderId   _id;
        TimePoint _time_point;
        Price     _price;
        Quantity  _remaining_quantity;
        OrderType _type;
        Side      _side;
    };

    //==============================================================================================
    // class : Order
    //==============================================================================================

    inline Order::Order(OrderType type, Side side, Price price, Quantity quantity) noexcept
        : _id(UUID())
        , _time_point(Clock::now())
        , _price(price)
        , _remaining_quantity(quantity)
        , _type(type)
        , _side(side)
    {}

    inline Order::Order(Side side, Quantity quantity) noexcept
        : _id(UUID())
        , _time_point(Clock::now())
        , _price(invalid_price)
        , _remaining_quantity(quantity)
        , _type(OrderType::None)
        , _side(side)
    {}

    constexpr auto Order::id() const noexcept -> OrderId
    {
        return _id;
    }

    constexpr auto Order::time_point() const noexcept -> TimePoint
    {
        return _time_point;
    }

    constexpr auto Order::price() const noexcept -> Price
    {
        return _price;
    }

    constexpr auto Order::remaining_quantity() const noexcept -> Quantity
    {
        return _remaining_quantity;
    }

    constexpr auto Order::type() const noexcept -> OrderType
    {
        return _type;
    }

    constexpr auto Order::side() const noexcept -> Side
    {
        return _side;
    }

    constexpr auto Order::is_market_order() const noexcept -> bool
    {
        return _price == invalid_price;
    }

    constexpr auto Order::fill(Quantity quantity) noexcept -> void
    {
        ensure(quantity <= _remaining_quantity, "Quantity exceeds remaining quantity");
        _remaining_quantity -= quantity;
    }

    constexpr auto Order::is_filled() const noexcept -> bool
    {
        return _remaining_quantity == 0;
    }
}
