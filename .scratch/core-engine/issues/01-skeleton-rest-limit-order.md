# 01: Skeleton: register an Instrument and rest a Limit Order

**What to build:** The smallest end-to-end Matching Engine. A developer can build the project, register Instruments, submit a Limit Order that does not Cross, see it come to rest, and inspect the Order Book. This sets up the build, the test framework and the one public test seam that every later ticket uses. See the parent spec at `.scratch/core-engine/spec.md` (user stories 1–7 and 46–50) and the vocabulary in `CONTEXT.md`.

**Blocked by:** None (can start immediately)

**Status:** done

- [x] The project builds with CMake as C++20; GoogleTest is fetched by CMake; the engine is a library target with a separate unit-test executable
- [x] An Instrument can be registered with an Instrument ID and a Tick; registering a duplicate Instrument ID is a setup error, not a Reject
- [x] Prices and Ticks are integers in 1/10,000 currency units; no floating point is used for Prices anywhere
- [x] Submitting a Limit Order (Side, Price, Quantity, Instrument, Order ID, Participant) that does not Cross produces exactly one Order Rested Event with Order ID, Side, Price and Remaining Quantity
- [x] Several Limit Orders at the same Price queue in arrival order within one Price Level
- [x] A read-only snapshot lists each Side's Price Levels best-first and each Price Level's Resting Orders in queue order (Order ID, Participant, Remaining Quantity)
- [x] Best Bid, Best Ask and Spread can be queried, including when one or both Sides are empty
- [x] A Resting Order can be looked up by Order ID
- [x] Unit tests exercise all of the above only through the Matching Engine's public interface; no test reaches into Order Book internals
- [x] The engine is deterministic: no clocks or randomness inside

## Comments

- 2026-10-13: Done test-first through the Matching Engine's public interface (9 unit tests). Crossing Limit Orders still rest without trading, and the Tick is accepted but not yet stored or validated; those are tickets 02 and 04.
