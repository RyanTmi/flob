#include <log/log.hpp>
#include <order_book/order_book.hpp>
#include <order_book/order_feeder.hpp>

#include <chrono>

using namespace flob;

auto print_order_book(const OrderBookInfos& ob) -> void
{
    Log::info("================ ORDER BOOK ===============");
    Log::info("{:^20} | {:^20}", "BIDS", "ASKS");
    Log::info("{:-^20}-+-{:-^20}", "", "");

    const auto max_depth = std::max(ob.bids.size(), ob.asks.size());
    for (usize i = 0; i < max_depth; ++i)
    {
        auto bid_str = i < ob.bids.size() ? std::format("{:>8} @ {:>8.2f}", ob.bids[i].quantity, ob.bids[i].price / 100.0) : "";
        auto ask_str = i < ob.asks.size() ? std::format("{:>8} @ {:>8.2f}", ob.asks[i].quantity, ob.asks[i].price / 100.0) : "";

        Log::info("{:20} | {:20}", bid_str, ask_str);
    }
    Log::info("===========================================");
}

auto main() -> int32
{
    OrderBookConfig cfg;
    cfg.session = new_york_session;

    OrderBook order_book(cfg);

    OrderFeeder feeder;

    // Generate many orders
    constexpr uint64 N = 500'000;

    const auto t0 = Clock::now();
    for (uint64 i = 0; i < N; ++i)
    {
        order_book.add_order(feeder.next());
    }
    const auto t1 = Clock::now();
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

    Log::info("Submitted {} orders in {} ms", N, ms);

    OrderBookInfos infos = order_book.infos();
    print_order_book(infos);
}
