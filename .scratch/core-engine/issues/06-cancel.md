# 06: Cancel

**What to build:** Participants can withdraw a Resting Order entirely by its Order ID. See the parent spec, `.scratch/core-engine/spec.md` (user stories 24–27). Note that "Cancel" here means full removal; ITCH's "Order Cancel" Message corresponds to Reduce (07), not to this.

**Blocked by:** 04 (Rejects for new Orders)

**Status:** ready-for-agent

- [ ] Cancel by Order ID removes the Resting Order and emits an Order Cancelled Event with the Order ID and the Remaining Quantity removed
- [ ] Every other Resting Order keeps its position; a Price Level emptied by the Cancel disappears and Best Bid and Best Ask update
- [ ] A Cancel for an Order ID that is not resting (never existed, or already filled, cancelled or discarded) is Rejected with "unknown Order ID" and changes nothing
- [ ] After a Cancel, the same Order ID can be used by a new Order
- [ ] Cancel finds the Order by ID without searching the whole Order Book
- [ ] The Reference Engine supports Cancel, and the fuzz generator sends Cancels for both live and unknown Order IDs (the small ID space makes both common)
- [ ] Unit tests cover Cancel at the front, middle and back of a Price Level, Cancel of the last Order at a Price Level, and Cancel after the Order was filled
