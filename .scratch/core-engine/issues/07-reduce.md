# 07: Reduce

**What to build:** Participants can shrink a Resting Order without losing its place in its Price Level. A Reduce corresponds one-to-one with an ITCH "Order Cancel" Message, which makes replay translation mechanical later. See the parent spec, `.scratch/core-engine/spec.md` (user stories 28–32).

**Blocked by:** 04 (Rejects for new Orders)

**Status:** ready-for-agent

- [ ] Reduce by Order ID and a Quantity lowers the Resting Order's Remaining Quantity by that Quantity and emits an Order Reduced Event with the Order ID, the Quantity removed and the new Remaining Quantity
- [ ] The reduced Order keeps its exact position in its Price Level, and later Trades at that Price Level still reach it in the same turn
- [ ] A Reduce Quantity that is zero, negative, or at least the Order's Remaining Quantity is Rejected with "invalid Reduce Quantity" and changes nothing (removing an Order entirely is what Cancel is for)
- [ ] A Reduce for an Order ID that is not resting is Rejected with "unknown Order ID"
- [ ] The Reference Engine supports Reduce, and the fuzz generator sends valid Reduces and invalid ones (amounts at or above Remaining Quantity, unknown Order IDs)
- [ ] Unit tests show that a reduced Order at the front of its Price Level still trades first
