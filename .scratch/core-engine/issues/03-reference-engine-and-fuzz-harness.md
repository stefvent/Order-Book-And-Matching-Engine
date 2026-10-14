# 03: Reference Engine and fuzz harness (Limit Orders)

**What to build:** An independent source of truth and a randomized check against it. A deliberately naive Reference Engine implements the same interface as the Matching Engine, and a seeded fuzz test drives both with random Limit Orders, failing on the first difference. Every later ticket extends both engines and the generator, so each feature is fuzz-checked as soon as it lands. See the parent spec, `.scratch/core-engine/spec.md` (user stories 51–55 and the fuzz section of Testing Decisions).

**Blocked by:** 02 (Crossing Limit Orders and Trades)

**Status:** ready-for-agent

- [ ] The Reference Engine exposes the same interface as the Matching Engine and shares no matching code with it; it favours obvious correctness (for example a flat list of Resting Orders scanned and sorted on every Request) over speed
- [ ] The fuzz generator is driven by a seeded PRNG and produces random Limit Orders across several Instruments and several Participants, with Prices drawn from a narrow band around a centre Price so Crosses are frequent
- [ ] After every Request the two engines' Events must be identical, and their snapshots of the affected Instrument must be identical; a full snapshot comparison of every Instrument runs periodically and at the end
- [ ] On the first mismatch, the test reports the seed, the index of the failing Request, the Request itself, and both engines' Events
- [ ] A short fixed-seed run is part of the normal test suite and finishes quickly
- [ ] A long run (configurable Request count and seed, 1M+ Requests) can be started on demand without running the rest of the suite
- [ ] The fuzz test lives in its own test executable, separate from the unit tests
