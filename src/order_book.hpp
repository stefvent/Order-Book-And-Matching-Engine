#pragma once

#include <deque>
#include <functional>
#include <map>
#include <optional>

#include "obme/matching_engine.hpp"

namespace obme {

// One Instrument's Resting Orders. Internal: reachable only through MatchingEngine.
class OrderBook {
 public:
  void rest(const LimitOrder& order);

  BookSnapshot snapshot() const;

  std::optional<Price> best_bid() const;
  std::optional<Price> best_ask() const;

  std::optional<RestingOrderView> find(Side side, Price price, OrderId order_id) const;

 private:
  using Queue = std::deque<RestingOrderView>;

  std::map<Price, Queue, std::greater<>> bids_;  // best (highest) first
  std::map<Price, Queue, std::less<>> asks_;     // best (lowest) first
};

}  // namespace obme
