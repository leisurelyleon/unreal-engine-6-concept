# The Verse Transactional Model: Infinite Scalability

**Status:** UE6 Core Architecture
**Topic:** Software Transactional Memory (STM) vs. Traditional Mutex Locks

## The Legacy Bottleneck (UE4 / UE5)
In traditional C++ game development, concurrent execution is handled via thread locks (`std::mutex`). If Player A and Player B both try to open a chest at the exact same millisecond, the engine locks the chest's state, processes Player A, unlocks it, locks it again, and processes Player B. In a Metaverse scenario with 10,000 players, this causes a catastrophic thread-locking cascade, crashing the server or plummeting the tick rate.

## The UE6 Verse Solution: Optimistic Concurrency
Unreal Engine 6 utilizes the Verse Virtual Machine, which natively implements **Software Transactional Memory (STM)**. 

Instead of locking memory, Verse assumes there will be no conflict (Optimistic Execution). 
1. **Snapshot:** A Verse fiber takes a snapshot of the world state.
2. **Compute:** It runs the game logic locally on that snapshot.
3. **Commit:** It attempts to swap the new state into the global world.
4. **Rollback:** If another fiber changed the state during step 2, Verse silently discards the work, rolls back, and instantly retries.

This completely eliminates thread deadlocks. Game logic can now scale infinitely across as many CPU cores as the server possesses, making seamless 10,000-player MMOs a native capability out of the box.
