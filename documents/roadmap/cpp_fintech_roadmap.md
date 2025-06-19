# C++ Fintech Career Roadmap

From Professional C# Developer to C++ Fintech Engineer

## Phase 1: Modern C++ Fundamentals

**Duration:** 4–6 weeks

**Goal:** Achieve proficiency in modern C++ (C++17/C++20) with focus on performance and safety.

### Core Language

- [ ] C++ syntax: variables, loops, conditionals
- [ ] Value vs reference semantics
- [ ] Functions: overloads, default args
- [ ] Structs vs Classes
- [ ] Namespaces & scope resolution

### Memory Management

- [ ] Stack vs Heap
- [ ] new / delete (raw pointers)
- [ ] RAII: Resource Acquisition Is Initialization
- [ ] Smart pointers: unique_ptr, shared_ptr, weak_ptr
- [ ] std::move and rvalue references

### Object-Oriented Programming

- [ ] Constructors & destructors
- [ ] Inheritance & virtual functions
- [ ] Abstract classes & interfaces
- [ ] The "Rule of 0/3/5"

### STL & Containers

- [ ] std::vector, std::deque, std::list
- [ ] std::map, std::unordered_map
- [ ] std::set, std::unordered_set
- [ ] std::string, std::optional, std::variant

### Templates

- [ ] Function templates
- [ ] Class templates
- [ ] Template specialization
- [ ] SFINAE & concepts (C++20)

### Error Handling

- [ ] Exceptions
- [ ] noexcept, try/catch best practices
- [ ] std::error_code and alternatives

## Phase 2: Advanced C++ & Performance

**Duration:** 6–8 weeks

**Goal:** Learn performance tuning, multithreading, and low-latency system design.

### Multithreading & Concurrency

- [ ] std::thread and thread management
- [ ] Mutexes, locks, std::lock_guard
- [ ] Atomic operations & memory ordering
- [ ] Thread pools & futures
- [ ] Lock-free programming basics

### Performance Optimization

- [ ] CPU cache behavior, cache lines
- [ ] False sharing & memory alignment
- [ ] Loop unrolling, inlining
- [ ] Copy elision & return value optimization (RVO)
- [ ] Custom allocators & memory pools

### Debugging & Profiling

- [ ] Valgrind, AddressSanitizer
- [ ] gdb / lldb basics
- [ ] perf / VTune / Instruments (Mac)
- [ ] Measuring latency & throughput

## Phase 3: Fintech-Specific Systems Programming

**Duration:** 6–10 weeks

**Goal:** Understand how real-time trading systems are built with C++.

### Networking

- [ ] TCP & UDP socket programming
- [ ] Non-blocking I/O with epoll/select
- [ ] Boost.Asio (async networking)
- [ ] Multicast (for market data)

### Financial Protocols

- [ ] FIX protocol basics (tags, sessions, messages)
- [ ] FAST & SBE encoding
- [ ] Write a simple FIX parser
- [ ] Work with QuickFIX / QuickFIX/n

### Market Infrastructure

- [ ] Order book implementation (limit book)
- [ ] Matching engine fundamentals
- [ ] Simulating an exchange feed
- [ ] Latency benchmarking & tuning

### Interprocess Communication (IPC)

- [ ] Shared memory (POSIX / Boost.Interprocess)
- [ ] Pipes and UNIX domain sockets
- [ ] ZeroMQ / nanomsg

### Time Handling

- [ ] High-resolution clocks
- [ ] std::chrono + hardware clocks
- [ ] Time sync (NTP, PTP)
- [ ] Handling timestamps in trading logs

## Phase 4: Financial Theory & Quantitative Concepts

**Duration:** Parallel track (~2–3 months)

**Goal:** Learn the financial logic behind what you’re building.

### Financial Instruments

- [ ] Stocks, ETFs, Bonds, Commodities
- [ ] Options: calls, puts, spreads, greeks
- [ ] Futures & margin trading
- [ ] Derivatives: swaps, CDS, etc.

### Pricing Models

- [ ] Black-Scholes model
- [ ] Monte Carlo simulation
- [ ] Binomial trees
- [ ] Greeks (Delta, Gamma, Vega, Theta)

### Trading Concepts

- [ ] Market vs limit orders
- [ ] Market microstructure
- [ ] Order flow, latency arbitrage
- [ ] Market making vs arbitrage vs trend following

### Quantitative Tools

- [ ] Time-series analysis
- [ ] Mean reversion, momentum strategies
- [ ] Backtesting frameworks
- [ ] QuantLib (C++)

## Phase 5: Tooling & Real-World Preparation

**Duration:** Ongoing

**Goal:** Build and polish real-world projects and be interview-ready.

### Environment & Tooling

- [ ] CMake: build system
- [ ] g++ / clang++ / MSVC setup
- [ ] Unit testing with Google Test
- [ ] Git / GitHub
- [ ] Docker (for reproducible backtesting environments)

### Sample Projects

- [ ] Real-time FIX engine (client + server)
- [ ] Multithreaded order book simulator
- [ ] UDP market data feed parser
- [ ] C++ backtesting engine
- [ ] Trade replay + P&L visualizer

### Interview & Job Prep

- [ ] C++ trivia & deep internals (vtable, memory layout)
- [ ] Leetcode-style C++ problems
- [ ] Concurrency & optimization questions
- [ ] System design: low-latency trading architecture
- [ ] Mock interviews (Algo + Design)

## Bonus: USA Fintech Context

### Companies to Target

- [ ] Jane Street
- [ ] Hudson River Trading (HRT)
- [ ] Jump Trading
- [ ] IMC Trading
- [ ] DRW
- [ ] Citadel Securities
- [ ] SIG (Susquehanna)
- [ ] Two Sigma

### Bonus Learning

- [ ] US Market structure: NASDAQ, NYSE, SIP feeds
- [ ] Regulation: SEC, FINRA, Reg NMS
- [ ] Colocation & exchange connectivity
