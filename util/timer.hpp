#pragma once

#include <iostream>
#include <chrono>
#include <thread>

namespace util
{
    struct timer;

    inline void pause(float sec);
} // namespace util

    //DEFINTIONS
struct util::timer{
    std::chrono::high_resolution_clock::time_point begin;

public:
    inline void start()
    {
        this->begin = std::chrono::high_resolution_clock::now();
    }
    inline int64_t stop()
    {
        int64_t dur = (std::chrono::high_resolution_clock::now() - this->begin).count();
        std::clog << "Took " << dur << " nanoseconds\n";
        return dur;
    }
};

void util::pause(float seconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long>(seconds * 1000)));
}
