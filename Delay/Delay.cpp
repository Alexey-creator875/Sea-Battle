#include "Delay.h"

#include <chrono>
#include <random>
#include <thread>

namespace Delay {
void Delay(int delay) {
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}

int GetRandomDelay(int minDelay, int maxDelay) {
    std::random_device r{};
    std::default_random_engine randomEngine(r());
    std::uniform_int_distribution<int> delay(minDelay, maxDelay);

    return delay(randomEngine);
}
}  // namespace Delay
