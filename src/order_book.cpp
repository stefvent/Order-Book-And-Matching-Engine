#include "order_book.hpp"

namespace obme {

namespace {

template <typename Levels>
std::optional<RestingOrderView> find_in(const Levels& levels, Price price, OrderId order_id) {
  auto level = levels.find(price);
  if (level == levels.end()) return std::nullopt;
  for (const auto& resting : level->second) {
    if (resting.order_id == order_id) return resting;
  }
  return std::nullopt;
}

template <typename Levels>
std::vector<PriceLevelView> view(const Levels& levels) {
  std::vector<PriceLevelView> out;
  for (const auto& [price, queue] : levels) {
    out.push_back({.price = price, .orders = {queue.begin(), queue.end()}});
  }
  return out;
}

}  // namespace

void OrderBook::rest(const LimitOrder& order) {
  RestingOrderView resting{
      .order_id = order.order_id, .participant = order.participant, .remaining = order.quantity};
  if (order.side == Side::Buy) {
    bids_[order.price].push_back(resting);
  } else {
    asks_[order.price].push_back(resting);
  }
}

BookSnapshot OrderBook::snapshot() const {
  return {.bids = view(bids_), .asks = view(asks_)};
}

std::optional<Price> OrderBook::best_bid() const {
  if (bids_.empty()) return std::nullopt;
  return bids_.begin()->first;
}

std::optional<Price> OrderBook::best_ask() const {
  if (asks_.empty()) return std::nullopt;
  return asks_.begin()->first;
}

std::optional<RestingOrderView> OrderBook::find(Side side, Price price, OrderId order_id) const {
  return side == Side::Buy ? find_in(bids_, price, order_id) : find_in(asks_, price, order_id);
}

}  // namespace obme
