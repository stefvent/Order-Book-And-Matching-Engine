#pragma once

#include <ostream>

#include "obme/matching_engine.hpp"

namespace obme::test {

inline constexpr InstrumentId kInstrument = 1;
inline constexpr ParticipantId kAlice = 1;
inline constexpr ParticipantId kBob = 2;

// Prices are integers in 1/10,000 currency units; cents(1002) is $10.02.
constexpr Price cents(std::int64_t c) { return c * 100; }

inline LimitOrder limit(OrderId id, Side side, Price price, Quantity quantity,
                        ParticipantId participant = kAlice, InstrumentId instrument = kInstrument) {
  return LimitOrder{.order_id = id,
                    .instrument = instrument,
                    .participant = participant,
                    .side = side,
                    .price = price,
                    .quantity = quantity};
}

}  // namespace obme::test

namespace obme {

// Readable gtest failure output for Events.
inline std::ostream& operator<<(std::ostream& os, Side side) {
  return os << (side == Side::Buy ? "Buy" : "Sell");
}

inline std::ostream& operator<<(std::ostream& os, const OrderRested& e) {
  return os << "OrderRested{id=" << e.order_id << ", " << e.side << ", price=" << e.price
            << ", remaining=" << e.remaining << "}";
}

inline std::ostream& operator<<(std::ostream& os, const RestingOrder& o) {
  return os << "RestingOrder{id=" << o.order_id << ", instrument=" << o.instrument
            << ", participant=" << o.participant << ", " << o.side << ", price=" << o.price
            << ", remaining=" << o.remaining << "}";
}

inline std::ostream& operator<<(std::ostream& os, const RestingOrderView& o) {
  return os << "{id=" << o.order_id << ", participant=" << o.participant
            << ", remaining=" << o.remaining << "}";
}

inline std::ostream& operator<<(std::ostream& os, const PriceLevelView& level) {
  os << "  " << level.price << ":";
  for (const auto& o : level.orders) os << " " << o;
  return os << "\n";
}

inline std::ostream& operator<<(std::ostream& os, const BookSnapshot& book) {
  os << "\nasks (best first):\n";
  for (const auto& level : book.asks) os << level;
  os << "bids (best first):\n";
  for (const auto& level : book.bids) os << level;
  return os;
}

}  // namespace obme
