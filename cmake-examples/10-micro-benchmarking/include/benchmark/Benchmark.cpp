#include "benchmark/Benchmark.h"
#include <iomanip>
#include <sstream>

Benchmark::Benchmark(std::string name)
    : m_name(std::move(name)), m_duration(0), m_hasEnded(false) {}

void Benchmark::start() {
    m_startTime = std::chrono::high_resolution_clock::now();
    m_hasEnded = false;
}

void Benchmark::end() {
    if (!m_hasEnded) {
        auto endTime = std::chrono::high_resolution_clock::now();
        m_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - m_startTime);
        m_hasEnded = true;
    }
}

std::string Benchmark::toString() const {
    if (!m_hasEnded) {
        return "Benchmark '" + m_name + "' has not ended yet.";
    }

    auto us = std::chrono::duration_cast<std::chrono::microseconds>(m_duration);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(m_duration);

    // Human-readable s:ms:us format
    auto s_part = std::chrono::duration_cast<std::chrono::seconds>(m_duration);
    auto ms_part = std::chrono::duration_cast<std::chrono::milliseconds>(m_duration - s_part);
    auto us_part = std::chrono::duration_cast<std::chrono::microseconds>(m_duration - s_part - ms_part);

    std::stringstream ss;
    ss << "--- Benchmark: '" << m_name << "' ---\n"
       << "  Microseconds: " << us.count() << " µs\n"
       << "  Milliseconds: " << ms.count() << " ms\n"
       << "  Human-readable: "
       << std::setfill('0') << std::setw(2) << s_part.count() << "s : "
       << std::setfill('0') << std::setw(3) << ms_part.count() << "ms : "
       << std::setfill('0') << std::setw(3) << us_part.count() << "µs\n"
       << "-------------------------------------";

    return ss.str();
}