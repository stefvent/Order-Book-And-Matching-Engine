# 08: Replace

**What to build:** Participants can change a Resting Order's Price or raise its Quantity. A Replace is a Cancel of the old Order plus a new Limit Order under a new Order ID, so it loses Price-Time Priority and can Cross. A Replace corresponds one-to-one with an ITCH "Order Replace" Message. See the parent spec, `.scratch/core-engine/spec.md` (user stories 33–39).

**Blocked by:** 02 (Crossing Limit Orders and Trades), 06 (Cancel)

**Status:** ready-for-agent

- [ ] A Replace names the old Order ID, a new Order ID, a new Price and a new Quantity; the new Order keeps the old Order's Side, Instrument and Participant
- [ ] A valid Replace emits an Order Cancelled Event for the old Order followed by the new Order's Events, exactly as if a new Limit Order had been submitted; there is no separate "Order Replaced" Event
- [ ] The new Order goes to the back of its Price Level, even when the Price is unchanged
- [ ] A Replace whose new Price Crosses trades as an Aggressor immediately, and any Remaining Quantity rests
- [ ] A Replace is validated in full before any state changes. If the old Order ID is not resting, the new Order ID is already resting, the new Price is invalid or off-Tick, or the new Quantity is not positive, the Replace is Rejected and the old Order stays exactly where it was
- [ ] The Reference Engine supports Replace, and the fuzz generator sends valid and invalid Replaces, including ones that Cross
- [ ] Unit tests cover losing priority at an unchanged Price, moving to a new Price, crossing on Replace, and each Reject path leaving the old Order untouched
