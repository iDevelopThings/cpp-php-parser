//
// Created by Sam on 10/06/2023.
//

#ifndef INTERPRETER_TIMER_H
#define INTERPRETER_TIMER_H


#include <map>
#include <iostream>
#include <chrono>
#include <functional>

inline std::map<std::string, uint64_t> times = {};

inline uint64_t nanos() {
    uint64_t ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count();
    return ns;
}

inline std::function<void()> time(const std::string &name) {
    auto start = std::chrono::high_resolution_clock::now();

    times[name] = nanos();

    // Return a function that will stop the timer
    auto f = [&start, &name]() {
        auto end = nanos();
        auto dur = end - times[name];

        auto s_time = dur / 1000000000;
        auto ms_time = dur / 1000000;
        auto us_time = dur / 1000;

        std::cout << "[" + name + "]: Took: ";

        if (s_time > 0) {
            std::cout << s_time << "s" << std::endl;
            return;
        }
        if (ms_time > 0) {
            std::cout << ms_time << "ms" << std::endl;
            return;
        }
        if (us_time > 0) {
            std::cout << us_time << "us" << std::endl;
            return;
        }

        std::cout << us_time << "us" << std::endl;
    };

    return f;
}


#endif //INTERPRETER_TIMER_H
