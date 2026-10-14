# Spec: Core Matching Engine

Status: ready-for-agent

## Problem Statement

I want a resume project that proves I understand how an exchange works and can write correct, performance-critical C++. Right now there is no code at all. Before I can measure throughput, replay Nasdaq ITCH data or run a backtester, I need a Matching Engine that keeps an Order Book per Instrument and matches Orders under Price-Time Priority. Its behaviour must be exact enough that I can prove it correct against an independent Reference Engine, and its interface must be stable enough that I can later rewrite the internals for speed without rewriting the tests.

## Solution

A C++ library with a single public entry point, the Matching Engine. Instruments are registered up front, each with its own Tick. The caller submits Requests one at a time: a new Limit Order, a new Market Order, a Cancel, a Reduce or a Replace. Each Request is processed synchronously and returns an ordered list of Events describing exactly what happened: Trades, an Order coming to rest, an Order being cancelled, reduced or discarded, or a Reject with a reason. The caller can also take a read-only snapshot of any Order Book.

Correctness is shown in two ways. Hand-written GoogleTest scenarios cover each rule. A randomized fuzz test sends the same Requests to the real engine and to a deliberately naive Reference Engine, and checks that the two always agree.

## User Stories

### Setup

1. As a developer, I want to register an Instrument with an Instrument ID and a Tick before trading it, so that each Order Book knows which Prices are valid.
2. As a developer, I want registering the same Instrument ID twice to be a setup error, so that a misconfigured run fails loudly before any Requests are processed.
3. As a developer, I want the Matching Engine to hold one Order Book per registered Instrument, so that Orders for different Instruments never interact.
4. As a developer, I want a fresh Matching Engine to start with empty Order Books, so that every test and replay starts from a known state.

### Limit Orders

5. As a Participant, I want to submit a Limit Order with a Side, Price, Quantity, Instrument, Order ID and my Participant ID, so that I can buy or sell at a Price I choose.
6. As a Participant, I want a Limit Order that does not Cross to become a Resting Order at its Price, so that it can trade later.
7. As a Participant, I want an Order Rested Event when my Order comes to rest, reporting its Order ID, Side, Price and Remaining Quantity, so that I know it is in the Order Book.
8. As a Participant, I want a Limit Order that Crosses to trade immediately against the opposite Side, so that I am not left waiting when a counterparty is already there.
9. As a Participant, I want an Aggressor Buy to trade against the lowest Ask first, and an Aggressor Sell against the highest Bid first, so that I always get the best available Price.
10. As a Participant, I want an Aggressor to trade against several Price Levels in turn, never beyond its own limit Price, so that I never pay more (or receive less) than I asked for.
11. As a Participant with a Resting Order, I want orders at my Price Level to trade in arrival order, so that arriving earlier is rewarded under Price-Time Priority.
12. As a Participant, I want every Trade to happen at the Resting Order's Price, so that an Aggressor priced better than the book gets the book's Price.
13. As a Participant, I want any Remaining Quantity of my Limit Order to rest at my limit Price after it has traded, so that a partially traded Order keeps working.
14. As a Participant, I want a Resting Order that has partly traded to keep its place at the front of its Price Level, so that a partial Trade doesn't cost me priority.
15. As a Participant, I want a Resting Order whose Remaining Quantity reaches zero to leave the Order Book, so that filled Orders can't trade again.
16. As a Participant, I want a Price Level with no Resting Orders left to disappear, so that the Best Bid and Best Ask always reflect real liquidity.

### Market Orders

17. As a Participant, I want to submit a Market Order with a Side, Quantity, Instrument, Order ID and my Participant ID, so that I can trade immediately at whatever Prices are available.
18. As a Participant, I want a Market Order to work through the opposite Side Price Level by Price Level until it is filled or the Side is empty, so that I get as much of my Quantity as the book can offer.
19. As a Participant, I want a Market Order's unfilled Remaining Quantity to be discarded, with an Order Discarded Event reporting how much, so that a Market Order never rests and I know how much didn't trade.
20. As a Participant, I want a Market Order arriving at an empty opposite Side to produce an Order Discarded Event for its full Quantity rather than a Reject, so that "nothing to trade against" is distinguishable from "invalid Request".

### Trades

21. As a Participant, I want each Trade Event to report the Instrument, Price, Quantity, the Aggressor's Order ID and Participant, the Resting Order's Order ID and Participant, and the Aggressor's Side, so that both parties can attribute it.
22. As a Participant, I want each Trade Event to report the Remaining Quantity of both Orders after the Trade, so that I can tell when an Order is filled without keeping my own records.
23. As the backtester (future), I want Trades between Orders of the same Participant to be allowed and reported like any other Trade, so that Self-Trades are visible rather than silently changing behaviour.

### Cancel

24. As a Participant, I want to Cancel a Resting Order by its Order ID, so that I can withdraw it.
25. As a Participant, I want an Order Cancelled Event reporting the Order ID and the Remaining Quantity removed, so that I know exactly what was withdrawn.
26. As a Participant, I want a Cancel for an Order ID that is not resting, whether it never existed or has already been filled, cancelled or discarded, to be Rejected with reason "unknown Order ID", so that a Cancel that lost a race with a Trade is reported cleanly.
27. As a Participant, I want a Cancel to leave every other Resting Order's position unchanged, so that other Participants' priority isn't affected.

### Reduce

28. As a Participant, I want to Reduce a Resting Order by a given Quantity, so that I can shrink it without giving up its place in the queue.
29. As a Participant, I want an Order Reduced Event reporting the Order ID, the Quantity removed and the new Remaining Quantity, so that I can confirm the change.
30. As a Participant, I want a Reduce to keep the Order's position in its Price Level, so that shrinking an Order never costs priority.
31. As a Participant, I want a Reduce whose Quantity is zero, negative, or at least the Order's Remaining Quantity to be Rejected, so that Reduce never becomes a hidden Cancel; removing an Order entirely is what Cancel is for.
32. As the ITCH replayer (future), I want a Reduce Request to correspond one-to-one with an ITCH "Order Cancel" Message, so that replay translation is mechanical.

### Replace

33. As a Participant, I want to Replace a Resting Order with a new Order ID, new Price and new Quantity, so that I can change Price or raise Quantity.
34. As a Participant, I want a Replace to produce an Order Cancelled Event for the old Order, followed by the Events of the new Order, so that a Replace is visibly a Cancel plus a new Order.
35. As a Participant, I want the new Order created by a Replace to go to the back of its Price Level, so that Replace never jumps the queue.
36. As a Participant, I want the new Order created by a Replace to keep the old Order's Side, Instrument and Participant, so that Replace can't change who is trading what.
37. As a Participant, I want a Replace whose new Price Crosses to trade as an Aggressor immediately, so that Replace behaves exactly like submitting a new Limit Order.
38. As a Participant, I want a Replace to be all-or-nothing, so that if any rule is broken (unknown old Order ID, new Order ID already resting, off-Tick Price, zero Quantity) it is Rejected and the old Order stays where it was.
39. As the ITCH replayer (future), I want a Replace Request to correspond one-to-one with an ITCH "Order Replace" Message, so that replay translation is mechanical.

### Rejects

40. As a Participant, I want an Order for an unregistered Instrument to be Rejected, so that typos in Instrument IDs are caught.
41. As a Participant, I want a Limit Order whose Price is not positive or not a whole number of Ticks to be Rejected, so that the Order Book only ever holds valid Prices.
42. As a Participant, I want an Order with a Quantity that is not positive to be Rejected, so that zero-size Orders can't sit in the book.
43. As a Participant, I want an Order whose Order ID matches any currently Resting Order, on any Instrument, to be Rejected, so that every Cancel, Reduce or Replace refers to exactly one Order.
44. As a Participant, I want each Reject Event to carry the Request's identifying Order ID and a specific reason code, so that I can tell which rule was broken.
45. As a developer, I want a Reject to leave every Order Book exactly as it was, so that invalid Requests never have partial effects.

### Observing the book

46. As a developer, I want to take a read-only snapshot of an Order Book, listing each Side's Price Levels in priority order and each Price Level's Resting Orders in queue order (Order ID, Participant, Remaining Quantity), so that tests can assert on the full state.
47. As a developer, I want to query the Best Bid, Best Ask and Spread of an Instrument, including the case where a Side is empty, so that the most common checks are one call.
48. As a developer, I want to look up a Resting Order by Order ID, so that tests and the future replayer can confirm an Order's current state.

### Determinism and correctness

49. As a developer, I want the Matching Engine to be fully deterministic, with arrival order defined by the order Requests are submitted and no clocks or randomness inside, so that the same Requests always produce the same Events.
50. As a developer, I want the Events from each Request to be returned in the order they happened, so that a Trade sequence can be replayed and audited.
51. As a developer, I want a naive Reference Engine behind the same interface, so that the fuzz test has an independent source of truth.
52. As a developer, I want a seeded fuzz test that sends millions of random Requests to both engines and compares Events and snapshots after every Request, so that I can claim "verified against a reference implementation with 1M+ randomized operations".
53. As a developer, I want a failing fuzz run to print the seed, the index of the failing Request, the Request itself and both engines' Events, so that a failure can be reproduced exactly.
54. As a developer, I want a short fuzz run in the normal test suite and a long one available on demand, so that everyday test runs stay fast.
55. As a developer, I want the Order Book's internals hidden behind the Matching Engine interface, so that I can replace them during the performance phase without changing any test.

## Implementation Decisions

**Modules**

- **Matching Engine** (public). Owns the Instrument registry and one Order Book per Instrument. It validates every Request, turns broken rules into Rejects, routes valid Requests to the right Order Book, and returns Events. It also owns the engine-wide Order ID → Resting Order lookup used by Cancel, Reduce and Replace.
- **Order Book** (internal, one per Instrument). Holds both Sides as Price Levels ordered best-first, each Price Level being a FIFO queue of Resting Orders. It performs matching. It has no public test surface; it is only reachable through the Matching Engine. The first version uses standard containers (an ordered map of Price → Price Level, a list-like queue per Price Level). The performance phase will replace these.
- **Reference Engine** (test-only). A deliberately simple implementation of the same interface, for example a flat list of Resting Orders per Instrument that is scanned and sorted on every Request. It favours obvious correctness over speed and must share no matching code with the real engine.
- **Fuzz harness** (test-only). Generates seeded random Request sequences, drives both engines, and compares their outputs.

**Representation**

- A Price is an integer in fixed-point units of 1/10,000 of a currency unit, matching ITCH's Price(4) fields. Floating point is never used for Prices.
- A Tick is a positive integer in the same units (for example 100 for $0.01). A Price is valid for an Instrument when it is positive and a whole multiple of that Instrument's Tick.
- Quantity is a positive integer. Order IDs are 64-bit unsigned integers chosen by the caller (ITCH supplies them, and synthetic tests assign them). Participant and Instrument IDs are small integers chosen by the caller.
- Order IDs only need to be unique among currently Resting Orders. An ID whose Order has been filled, cancelled or discarded may be reused. This avoids remembering every ID ever seen and matches what the duplicate check can see cheaply.

**Interface contract**

- Instrument registration is a setup step, separate from Requests. Registering a duplicate Instrument ID is a programming error, not a Reject.
- There is one entry point per Request kind: new Limit Order, new Market Order, Cancel, Reduce and Replace. Each processes its Request completely before returning and produces that Request's Events in order. The exact delivery mechanism (returned list, caller-supplied buffer, or listener) is left to implementation, but it must keep that order and must not allocate per Event in a way that blocks later optimization.
- The Event kinds are: Trade, Order Rested, Order Cancelled, Order Reduced, Order Discarded and Reject. There is deliberately no separate "Order Replaced" Event, because a Replace is an Order Cancelled Event followed by the new Order's Events.
- A valid new Order produces, in order: zero or more Trades, then exactly one of Order Rested (Limit Order with Remaining Quantity), Order Discarded (Market Order with Remaining Quantity), or nothing more (filled).
- Reject reasons are a closed set: unknown Instrument, invalid Price (not positive or off-Tick), invalid Quantity, duplicate Order ID, unknown Order ID, and invalid Reduce Quantity.
- A Replace is validated in full before any state changes. If it is valid, the old Order is cancelled and the new Order is processed exactly as a new Limit Order from the same Participant on the same Side and Instrument.
- Self-Trades are allowed and are not marked specially.

**Build**

- C++20, built with CMake. GoogleTest is fetched by CMake. The engine is a library target. Unit tests and the fuzz test are separate test executables, so the long fuzz run can be started on its own.

## Testing Decisions

- **What a good test looks like.** Each test submits Requests through the Matching Engine interface and asserts only on the Events returned and on Order Book snapshots or queries. No test touches Order Book internals, container types or private state. A test should read like the worked examples in the glossary discussion: a starting book, one Request, the expected Events and the resulting book.
- **One seam.** The Matching Engine's public interface is the only test seam. Unit tests, the fuzz test and the future benchmarks all use it, so the performance phase can rewrite the internals freely.
- **Unit tests (GoogleTest)** are grouped by rule:
  - resting without crossing
  - crossing a single Price Level
  - walking several Price Levels
  - Price-Time Priority within a Price Level
  - Trade at the Resting Order's Price
  - a partial Trade keeping its position
  - an empty Price Level disappearing
  - Market Order fully filled, partially filled, and arriving at an empty Side
  - Cancel and Reduce, including position preservation
  - Replace, including losing priority, crossing on Replace, and atomic Reject
  - every Reject reason, each leaving the book unchanged
  - multiple Instruments staying isolated
  - Self-Trades
- **Fuzz test.**
  - Requests come from a seeded PRNG with weighted Request kinds.
  - Prices are drawn from a narrow band around a centre Price, so Crosses are frequent.
  - The Order ID space is small, so duplicate and unknown IDs come up regularly.
  - A few Participants are used, so Self-Trades occur, and a few Instruments.
  - A fraction of Requests are deliberately invalid: off-Tick Prices, zero Quantities, and Reduces of at least the Remaining Quantity.
  - After every Request, both engines' Events must be identical, and their snapshots must be identical for the affected Instrument.
  - A full snapshot comparison of every Instrument runs periodically and at the end.
  - The normal suite runs a short fixed-seed run. The long run (1M+ Requests, any seed) is opt-in.
- **Prior art.** There is none in this repo yet. These tests set the pattern that later specs (performance, ITCH replay, backtester) should follow.

## Out of Scope

- Performance work: throughput and latency benchmarks, tick-indexed arrays, object pools, intrusive lists and profiling. These belong to a separate performance spec, and this spec only requires that its interface not block them.
- Parsing Nasdaq ITCH or recording and replaying a Coinbase feed. This spec only makes Reduce and Replace map one-to-one onto ITCH Messages.
- Python bindings, the backtester, Strategies, Signals, Slippage and transaction cost modelling.
- Self-Trade prevention.
- Any other order type or time-in-force: immediate-or-cancel, fill-or-kill, stop, iceberg, hidden, post-only, day or good-till-cancelled.
- Auctions (opening and closing crosses), trading sessions and halts.
- Publishing market data feeds (L1 or L2) from the engine.
- Networking, persistence, recovery and multithreading. The engine is single-threaded and in-memory.
- Participant positions, P&L, fees and risk checks.

## Further Notes

- Vocabulary follows `CONTEXT.md`. In particular, Trade is the name for a match ("fill" is never a noun), Cancel removes an Order and Reduce shrinks it (ITCH uses "Cancel" for what this project calls Reduce), Request is the name for inputs and Event for outputs, and Message is reserved for feed records.
- Rejecting a Reduce that would take an Order to zero is a deliberate choice: it keeps Reduce and Cancel distinct. ITCH sends a full removal as a separate "Order Delete" Message, so replay is unaffected.
- Allowing Order ID reuse after an Order is no longer resting keeps the duplicate check cheap. The Reference Engine must follow the same rule, or the fuzz test will report false mismatches.
- The project brief says to ship a working, tested core engine with a clean README early and add it to the resume then, rather than waiting for the stretch phases. This spec is that milestone.
