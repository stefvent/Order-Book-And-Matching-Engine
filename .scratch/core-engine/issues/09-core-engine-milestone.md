# 09: Core engine milestone: 1M+ fuzz run and README

**What to build:** The shippable core engine. Every Request kind is covered by the fuzz test, a long run proves the engine agrees with the Reference Engine over more than a million randomized Requests, and a README explains the project to a recruiter or interviewer. This is the version that goes on the resume (see "Further Notes" in `.scratch/core-engine/spec.md`).

**Blocked by:** 05 (Market Orders), 06 (Cancel), 07 (Reduce), 08 (Replace)

**Status:** ready-for-agent

- [ ] The fuzz generator mixes every Request kind (Limit Order, Market Order, Cancel, Reduce, Replace) together with valid and deliberately invalid Requests, across several Instruments and Participants
- [ ] A long fuzz run of at least 1M Requests passes for several different seeds with zero mismatches, and the README records the command, the seeds and the Request counts used
- [ ] The README explains what an Order Book and Price-Time Priority are, using the vocabulary in `CONTEXT.md`
- [ ] The README lists the engine's guarantees: the supported Request and Event kinds, the Reject reasons, determinism, and the Cancel/Reduce naming difference from ITCH
- [ ] The README explains how to build, run the unit tests, run the short fuzz test and run the long fuzz test
- [ ] The README states what is out of scope for now (performance work, ITCH replay, backtester) and points to them as next phases
- [ ] A clean checkout builds and passes all tests by following only the README
