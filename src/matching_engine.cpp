#include "obme/matching_engine.hpp"

#include <stdexcept>
#include <string>

#include "order_book.hpp"

namespace obme {

MatchingEngine::MatchingEngine() = default;
MatchingEngine::~MatchingEngine() = default;
MatchingEngine::MatchingEngine(MatchingEngine&&) noexcept = default;
MatchingEngine& MatchingEngine::operator=(MatchingEngine&&) noexcept = default;

// The Tick is not stored yet: nothing validates Prices against it until Rejects exist.
void MatchingEngine::register_instrument(InstrumentId instrument, Price /*tick*/) {
  auto [_, inserted] = books_.emplace(instrument, std::make_unique<OrderBook>());
  if (!inserted) {
    throw std::invalid_argument("Instrument " + std::to_string(instrument) + " is already registered");
  }
}

std::vector<Event> MatchingEngine::submit(const LimitOrder& order) {
  books_.at(order.instrument)->rest(order);
  resting_[order.order_id] = {.instrument = order.instrument, .side = order.side, .price = order.price};
  return {OrderRested{.order_id = order.order_id,
                      .side = order.side,
                      .price = order.price,
                      .remaining = order.quantity}};
}

BookSnapshot MatchingEngine::snapshot(InstrumentId instrument) const {
  return books_.at(instrument)->snapshot();
}

std::optional<Price> MatchingEngine::best_bid(InstrumentId instrument) const {
  return books_.at(instrument)->best_bid();
}

std::optional<Price> MatchingEngine::best_ask(InstrumentId instrument) const {
  return books_.at(instrument)->best_ask();
}

std::optional<Price> MatchingEngine::spread(InstrumentId instrument) const {
  auto bid = best_bid(instrument);
  auto ask = best_ask(instrument);
  if (!bid || !ask) return std::nullopt;
  return *ask - *bid;
}

std::optional<RestingOrder> MatchingEngine::find_resting_order(OrderId order_id) const {
  auto it = resting_.find(order_id);
  if (it == resting_.end()) return std::nullopt;
  const Location& at = it->second;
  auto resting = books_.at(at.instrument)->find(at.side, at.price, order_id);
  if (!resting) return std::nullopt;
  return RestingOrder{.order_id = order_id,
                      .instrument = at.instrument,
                      .participant = resting->participant,
                      .side = at.side,
                      .price = at.price,
                      .remaining = resting->remaining};
}

}  // namespace obme
