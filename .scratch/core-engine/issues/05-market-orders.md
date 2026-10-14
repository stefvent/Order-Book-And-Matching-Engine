# 05: Market Orders

**What to build:** Participants can submit Market Orders that trade immediately at whatever Prices the opposite Side offers, with any unfilled part discarded rather than resting. See the parent spec, `.scratch/core-engine/spec.md` (user stories 17–20).

**Blocked by:** 03 (Reference Engine and fuzz harness)

**Status:** ready-for-agent

- [ ] A Market Order (Side, Quantity, Instrument, Order ID, Participant; no Price) trades against the opposite Side Price Level by Price Level under Price-Time Priority, at each Resting Order's Price
- [ ] A Market Order never rests: if Remaining Quantity is left after the opposite Side is exhausted, an Order Discarded Event reports the discarded Quantity after the Trades
- [ ] A filled Market Order emits Trades only, with no Order Discarded Event
- [ ] A Market Order arriving at an empty opposite Side produces one Order Discarded Event for its full Quantity, not a Reject
- [ ] If 04 has landed, Market Orders are subject to the same Instrument, Quantity and duplicate Order ID checks as Limit Orders; otherwise this ticket adds those checks when it is merged after 04
- [ ] The Reference Engine supports Market Orders, and the fuzz generator mixes them in with Limit Orders
- [ ] Unit tests cover filled, partially filled, and empty-Side Market Orders, including a Market Order that walks several Price Levels
