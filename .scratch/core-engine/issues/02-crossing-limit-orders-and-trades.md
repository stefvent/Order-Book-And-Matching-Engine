# 02: Crossing Limit Orders and Trades

**What to build:** Limit Orders that Cross now trade. An Aggressor matches the opposite Side under Price-Time Priority, walks several Price Levels up to its limit Price, and trades at each Resting Order's Price. Any Remaining Quantity rests. This is the core matching behaviour from the parent spec, `.scratch/core-engine/spec.md` (user stories 8–16, 21–23 and 3).

**Blocked by:** 01 (Skeleton: register an Instrument and rest a Limit Order)

**Status:** ready-for-agent

- [ ] A Buy priced at or above the Best Ask (or a Sell at or below the Best Bid) Crosses and produces Trades; otherwise it rests as in 01
- [ ] The Aggressor trades best Price first and, within a Price Level, earliest-arrived Resting Order first
- [ ] Every Trade happens at the Resting Order's Price, even when the Aggressor's limit Price is better
- [ ] The Aggressor walks several Price Levels in order and never trades beyond its own limit Price
- [ ] Each Trade Event reports Instrument, Price, Quantity, Aggressor Order ID and Participant, Resting Order ID and Participant, Aggressor Side, and both Orders' Remaining Quantity after the Trade
- [ ] An Aggressor's Remaining Quantity after matching rests at its limit Price and emits Order Rested after the Trades; a filled Aggressor emits nothing further
- [ ] A partly traded Resting Order keeps its place at the front of its Price Level
- [ ] A Resting Order whose Remaining Quantity reaches zero leaves the Order Book; an empty Price Level disappears, and Best Bid and Best Ask update accordingly
- [ ] Self-Trades (same Participant on both sides) are allowed and reported like any other Trade
- [ ] Orders on different Instruments never interact
- [ ] Unit tests cover each rule above with worked examples (a starting book, one Request, expected Events, resulting snapshot), including the walking-the-book example
