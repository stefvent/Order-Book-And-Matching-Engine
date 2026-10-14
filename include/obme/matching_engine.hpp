#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <unordered_map>
#include <variant>
#include <vector>

namespace obme {

// A Price is an integer in 1/10,000 currency units, matching ITCH's Price(4) fields.
using Price = std::int64_t;
using Quantity = std::int64_t;
using OrderId = std::uint64_t;
using InstrumentId = std::uint32_t;
using ParticipantId = std::uint32_t;

enum class Side { Buy, Sell };

struct LimitOrder {
  OrderId order_id;
  InstrumentId instrument;
  ParticipantId participant;
  Side side;
  Price price;
  Quantity quantity;
};

struct OrderRested {
  OrderId order_id;
  Side side;
  Price price;
  Quantity remaining;

  bool operator==(const OrderRested&) const = default;
};

using Event = std::variant<OrderRested>;

struct RestingOrder {
  OrderId order_id;
  InstrumentId instrument;
  ParticipantId participant;
  Side side;
  Price price;
  Quantity remaining;

  bool operator==(const RestingOrder&) const = default;
};

struct RestingOrderView {
  OrderId order_id;
  ParticipantId participant;
  Quantity remaining;

  bool operator==(const RestingOrderView&) const = default;
};

struct PriceLevelView {
  Price price;
  std::vector<RestingOrderView> orders;  // queue order, front first

  bool operator==(const PriceLevelView&) const = default;
};

struct BookSnapshot {
  std::vector<PriceLevelView> bids;  // best (highest) Price first
  std::vector<PriceLevelView> asks;  // best (lowest) Price first

  bool operator==(const BookSnapshot&) const = default;
};

class OrderBook;

class MatchingEngine {
 public:
  MatchingEngine();
  ~MatchingEngine();
  MatchingEngine(MatchingEngine&&) noexcept;
  MatchingEngine& operator=(MatchingEngine&&) noexcept;

  // Setup step, not a Request. Throws std::invalid_argument if the Instrument is already registered.
  void register_instrument(InstrumentId instrument, Price tick);

  std::vector<Event> submit(const LimitOrder& order);

  BookSnapshot snapshot(InstrumentId instrument) const;

  std::optional<Price> best_bid(InstrumentId instrument) const;
  std::optional<Price> best_ask(InstrumentId instrument) const;
  // Best Ask minus Best Bid; empty unless both Sides have Resting Orders.
  std::optional<Price> spread(InstrumentId instrument) const;

  std::optional<RestingOrder> find_resting_order(OrderId order_id) const;

 private:
  struct Location {
    InstrumentId instrument;
    Side side;
    Price price;
  };

  std::unordered_map<InstrumentId, std::unique_ptr<OrderBook>> books_;
  std::unordered_map<OrderId, Location> resting_;
};

}  // namespace obme
