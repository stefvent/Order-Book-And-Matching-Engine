#include <optional>

#include <gtest/gtest.h>

#include "obme/matching_engine.hpp"
#include "test_support.hpp"

using namespace obme;
using namespace obme::test;

TEST(TopOfBook, EmptyOrderBookHasNoBestBidBestAskOrSpread) {
  MatchingEngine engine;
  engine.register_instrument(kInstrument, cents(1));

  EXPECT_EQ(engine.best_bid(kInstrument), std::nullopt);
  EXPECT_EQ(engine.best_ask(kInstrument), std::nullopt);
  EXPECT_EQ(engine.spread(kInstrument), std::nullopt);
}

TEST(TopOfBook, BestBidIsHighestBidBestAskIsLowestAskAndSpreadIsTheirDifference) {
  MatchingEngine engine;
  engine.register_instrument(kInstrument, cents(1));
  engine.submit(limit(1, Side::Buy, cents(1000), 500));
  engine.submit(limit(2, Side::Buy, cents(999), 200));
  engine.submit(limit(3, Side::Sell, cents(1003), 200));
  engine.submit(limit(4, Side::Sell, cents(1002), 100));

  EXPECT_EQ(engine.best_bid(kInstrument), cents(1000));
  EXPECT_EQ(engine.best_ask(kInstrument), cents(1002));
  EXPECT_EQ(engine.spread(kInstrument), cents(2));
}

TEST(TopOfBook, OrderBookWithOnlyBidsHasBestBidButNoBestAskOrSpread) {
  MatchingEngine engine;
  engine.register_instrument(kInstrument, cents(1));
  engine.submit(limit(1, Side::Buy, cents(1000), 500));

  EXPECT_EQ(engine.best_bid(kInstrument), cents(1000));
  EXPECT_EQ(engine.best_ask(kInstrument), std::nullopt);
  EXPECT_EQ(engine.spread(kInstrument), std::nullopt);
}
