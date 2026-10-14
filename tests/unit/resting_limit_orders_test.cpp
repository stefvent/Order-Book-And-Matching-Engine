#include <vector>

#include <gtest/gtest.h>

#include "obme/matching_engine.hpp"
#include "test_support.hpp"

using namespace obme;
using namespace obme::test;

TEST(RestingLimitOrders, LimitOrderThatDoesNotCrossRestsAndReportsOrderRested) {
  MatchingEngine engine;
  engine.register_instrument(kInstrument, cents(1));

  auto events = engine.submit(limit(1, Side::Buy, cents(1000), 500));

  EXPECT_EQ(events, (std::vector<Event>{
                        OrderRested{.order_id = 1, .side = Side::Buy, .price = cents(1000), .remaining = 500},
                    }));
}

TEST(RestingLimitOrders, OrdersAtTheSamePriceQueueInArrivalOrder) {
  MatchingEngine engine;
  engine.register_instrument(kInstrument, cents(1));

  engine.submit(limit(1, Side::Buy, cents(1000), 500, kAlice));
  engine.submit(limit(2, Side::Buy, cents(1000), 200, kBob));
  engine.submit(limit(3, Side::Buy, cents(1000), 100, kAlice));

  EXPECT_EQ(engine.snapshot(kInstrument),
            (BookSnapshot{
                .bids = {{.price = cents(1000),
                          .orders = {{.order_id = 1, .participant = kAlice, .remaining = 500},
                                     {.order_id = 2, .participant = kBob, .remaining = 200},
                                     {.order_id = 3, .participant = kAlice, .remaining = 100}}}},
                .asks = {},
            }));
}

TEST(RestingLimitOrders, SnapshotListsEachSidesPriceLevelsBestFirst) {
  MatchingEngine engine;
  engine.register_instrument(kInstrument, cents(1));

  engine.submit(limit(1, Side::Buy, cents(999), 100));
  engine.submit(limit(2, Side::Sell, cents(1003), 200));
  engine.submit(limit(3, Side::Buy, cents(1000), 300));
  engine.submit(limit(4, Side::Sell, cents(1002), 400));
  engine.submit(limit(5, Side::Buy, cents(998), 500));

  EXPECT_EQ(engine.snapshot(kInstrument),
            (BookSnapshot{
                .bids = {{.price = cents(1000), .orders = {{.order_id = 3, .participant = kAlice, .remaining = 300}}},
                         {.price = cents(999), .orders = {{.order_id = 1, .participant = kAlice, .remaining = 100}}},
                         {.price = cents(998), .orders = {{.order_id = 5, .participant = kAlice, .remaining = 500}}}},
                .asks = {{.price = cents(1002), .orders = {{.order_id = 4, .participant = kAlice, .remaining = 400}}},
                         {.price = cents(1003), .orders = {{.order_id = 2, .participant = kAlice, .remaining = 200}}}},
            }));
}

TEST(RestingLimitOrders, RestingOrderCanBeFoundByOrderId) {
  MatchingEngine engine;
  engine.register_instrument(kInstrument, cents(1));
  engine.submit(limit(1, Side::Buy, cents(1000), 500, kAlice));
  engine.submit(limit(2, Side::Sell, cents(1002), 300, kBob));

  EXPECT_EQ(engine.find_resting_order(2), (RestingOrder{.order_id = 2,
                                                        .instrument = kInstrument,
                                                        .participant = kBob,
                                                        .side = Side::Sell,
                                                        .price = cents(1002),
                                                        .remaining = 300}));
}

TEST(RestingLimitOrders, UnknownOrderIdIsNotFound) {
  MatchingEngine engine;
  engine.register_instrument(kInstrument, cents(1));
  engine.submit(limit(1, Side::Buy, cents(1000), 500));

  EXPECT_EQ(engine.find_resting_order(99), std::nullopt);
}
