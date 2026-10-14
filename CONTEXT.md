# Order Book and Matching Engine

A limit order book and price-time priority matching engine, driven by synthetic orders and by replayed real market data, with a backtester on top.

## The book

**Instrument**:
A single tradable thing, such as one stock, that has its own Order Book.
_Avoid_: symbol, ticker, security, stock

**Order Book**:
All Resting Orders for one Instrument, split into a Buy Side and a Sell Side.
_Avoid_: LOB, limit order book, depth

**Side**:
Whether an Order buys or sells: Buy or Sell.
_Avoid_: direction

**Bid**:
A Resting Order on the Buy Side.
_Avoid_: buy order (when you mean a resting one)

**Ask**:
A Resting Order on the Sell Side.
_Avoid_: offer

**Price Level**:
All Resting Orders on one Side at one Price, queued in arrival order.
_Avoid_: level, tier, bucket

**Tick**:
The smallest allowed step between two Prices on an Instrument.
_Avoid_: increment, pip

**Best Bid**:
The highest Bid Price currently in the Order Book.

**Best Ask**:
The lowest Ask Price currently in the Order Book.

**Spread**:
The Best Ask minus the Best Bid.

## Orders

**Order**:
An instruction from a Participant to buy or sell a Quantity of an Instrument, identified by a unique Order ID.

**Participant**:
The party an Order belongs to, such as the backtester's strategy or the rest of the market.
_Avoid_: trader, user, account, owner, client

**Limit Order**:
An Order with a limit Price that trades only at that Price or better; any Remaining Quantity becomes a Resting Order.

**Market Order**:
An Order with no Price that trades against whatever the opposite Side offers; any Remaining Quantity is discarded and never rests.

**Cancel**:
A request that removes a Resting Order from the Order Book entirely.
_Avoid_: delete

**Reduce**:
A request that lowers a Resting Order's Remaining Quantity without changing its place in its Price Level.
_Avoid_: partial cancel, amend (and ITCH's "Order Cancel", which means Reduce)

**Replace**:
A Cancel of a Resting Order combined with a new Order under a new Order ID, used to change Price or raise Quantity. The new Order loses the old one's place under Price-Time Priority.
_Avoid_: modify, amend, cancel-replace

**Quantity**:
The number of units an Order is for.
_Avoid_: size, shares, volume, qty

**Remaining Quantity**:
The part of an Order's Quantity that has not yet traded.
_Avoid_: leaves quantity, open quantity, residual

**Resting Order**:
An Order waiting in the Order Book to be traded against.
_Avoid_: passive order, maker order, standing order

**Aggressor**:
The incoming Order that Crosses and trades against Resting Orders.
_Avoid_: taker, active order

## Matching

**Matching Engine**:
The component that accepts Requests, matches Orders against the Order Book for their Instrument, and emits Events.
_Avoid_: exchange, matcher

**Request**:
Anything sent into the Matching Engine: a new Order, a Cancel, a Reduce or a Replace.
_Avoid_: command, message, instruction

**Event**:
Anything the Matching Engine emits to say what happened, such as a Trade, a Reject, an Order coming to rest or a Cancel taking effect.
_Avoid_: report, message, notification

**Message**:
A record in a replayed market data feed such as Nasdaq ITCH. Messages are translated into Requests before they reach the Matching Engine.
_Avoid_: using it for Requests or Events

**Cross**:
An Order Crosses when it can trade immediately: a Buy priced at or above the Best Ask, or a Sell priced at or below the Best Bid. A Market Order Crosses whenever the opposite Side is non-empty.
_Avoid_: marketable, match (for this check)

**Trade**:
A single match between an Aggressor and one Resting Order, at the Resting Order's Price, for some Quantity. One Aggressor can produce several Trades. An Order whose Remaining Quantity reaches zero is "filled"; that word describes the Order's state and is never a noun.
_Avoid_: fill, execution, match

**Self-Trade**:
A Trade in which the Aggressor and the Resting Order belong to the same Participant. Self-Trades are currently allowed.
_Avoid_: wash trade (that is a regulatory term with a different meaning)

**Reject**:
The Matching Engine's refusal of a Request that breaks a rule, such as an off-Tick Price, a zero Quantity, a duplicate Order ID or an unknown Order ID, together with the reason. A Reject leaves the Order Book unchanged. A Market Order that finds the opposite Side empty is not a Reject; it simply produces no Trades.
_Avoid_: error, failure, invalid order

**Price-Time Priority**:
The rule that Resting Orders trade best Price first and, within a Price Level, earliest arrival first.
_Avoid_: FIFO matching
