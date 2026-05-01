# The Death of the Single Thread: Fiber-Based Execution

**Status:** UE6 Core Architecture
**Topic:** Tickless Graph Execution and Asymmetric Silicon

## The `Tick()` Problem
For decades, game engines relied on a monolithic `MainLoop` that called `Tick(DeltaTime)` on every single object in the world, one by one. If one object took too long to think, the entire game stuttered.

## The Fiber Dispatcher
UE6 destroys the `Tick()`. Instead, it uses a **Dependency Graph**. 
When an event happens, it spawns a "Fiber" (a lightweight user-space thread). Millions of fibers can exist simultaneously. 

The `FiberTaskDispatcher` intelligently routes these tasks based on modern Asymmetric hardware (like Intel Big.Little or the custom Zen 6 chips in Project Helix):
* **Performance Cores (P-Cores):** Handle rigid-body physics, rendering dispatch, and critical Verse transactions.
* **Efficiency Cores (E-Cores):** Handle audio mixing, background asset decompression, and asynchronous network replication.

The engine never sleeps, it never waits, and it never drops a frame due to a single heavy script.
