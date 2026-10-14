# 04: Rejects for new Orders

**What to build:** Invalid new Orders are refused cleanly. The Matching Engine emits a Reject Event with a specific reason code and leaves every Order Book exactly as it was. This introduces the Reject Event and its closed set of reasons, which Cancel, Reduce and Replace reuse later. See the parent spec, `.scratch/core-engine/spec.md` (user stories 40–45).

**Blocked by:** 03 (Reference Engine and fuzz harness)

**Status:** ready-for-agent

- [ ] A Reject Event carries the Request's identifying Order ID and a reason code from a closed set (unknown Instrument, invalid Price, invalid Quantity, duplicate Order ID, unknown Order ID, invalid Reduce Quantity); the last two are defined here but first used in 06 and 07
- [ ] An Order for an unregistered Instrument is Rejected with "unknown Instrument"
- [ ] A Limit Order whose Price is not positive, or not a whole multiple of the Instrument's Tick, is Rejected with "invalid Price"
- [ ] An Order whose Quantity is not positive is Rejected with "invalid Quantity"
- [ ] An Order whose Order ID matches any currently Resting Order on any Instrument is Rejected with "duplicate Order ID"; an Order ID whose Order is no longer resting may be reused
- [ ] A Rejected Request changes no Order Book; unit tests confirm this with before and after snapshots for every reason
- [ ] The Reference Engine implements the same rules, and the fuzz generator sends a fraction of deliberately invalid Limit Orders (off-Tick Prices, zero Quantities, duplicate Order IDs, unknown Instruments)
