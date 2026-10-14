#include <stdexcept>

#include <gtest/gtest.h>

#include "obme/matching_engine.hpp"
#include "test_support.hpp"

using namespace obme;
using namespace obme::test;

TEST(InstrumentRegistration, RegisteringTheSameInstrumentTwiceIsASetupError) {
  MatchingEngine engine;
  engine.register_instrument(kInstrument, cents(1));

  EXPECT_THROW(engine.register_instrument(kInstrument, cents(5)), std::invalid_argument);
}
