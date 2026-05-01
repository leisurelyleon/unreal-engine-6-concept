#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

namespace UE6 {
namespace Scheduling {

class FiberTaskDispatcher {
private:
    // Lock-free work-stealing queues would be used in production.
    // For this conceptual architecture, we mock the fiber pool distribution.
    std::queue<std::function<void()>> fiber_queue;
    std::mutex queue_mutex;
    std::condition_variable condition;
    std::vector<std::thread> asymmetric_worker_pool;
    std::atomic<bool> bIsEngineShuttingDown;

public:
    FiberTaskDispatcher(size_t p_cores, size_t e_cores) : bIsEngineShuttingDown(false) {
        size_t total_hardware_threads = p_cores + e_cores;
        std::cout << "[UE6-SCHEDULER] Booting Lock-Free Fiber Dispatcher across " 
                  << total_hardware_threads << " asymmetric cores...\n";

        for (size_t i = 0; i < total_hardware_threads; ++i) {
            asymmetric_worker_pool.emplace_back([this, i, p_cores]() {
                this->WorkerThreadLifecycle(i < p_cores);
            });
        }
    }

    ~FiberTaskDispatcher() {
        bIsEngineShuttingDown = true;
        condition.notify_all();
        for (std::thread& worker : asymmetric_worker_pool) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    // Dispatches a micro-task into the execution graph
    template<class F>
    void DispatchFiber(F&& task) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            fiber_queue.emplace(std::forward<F>(task));
        }
        condition.notify_one();
    }

private:
    void WorkerThreadLifecycle(bool is_performance_core) {
        while (!bIsEngineShuttingDown) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                condition.wait(lock, [this] { 
                    return bIsEngineShuttingDown || !fiber_queue.empty(); 
                });

                if (bIsEngineShuttingDown && fiber_queue.empty()) return;

                task = std::move(fiber_queue.front());
                fiber_queue.pop();
            }

            // Execute the fiber. P-Cores handle rendering/physics, E-Cores handle audio/background IO
            task();
        }
    }
};

} // namespace Scheduling
} // namespace UE6
