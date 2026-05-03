---
date: 2026-04-29
tags: [cpp, quant, roadmap, tracker]
---

# C++ Quant Dev Roadmap

> **Goal:** Become a proficient C++ quant developer. Daily ~30-min deep-dive lessons, sequential curriculum from beginner to quant-ready.

> **How this works:** A scheduled task runs daily at 4am. It reads `current_day` below, generates that lesson as a new note in the matching phase folder, marks the day complete here, and increments `current_day`.

## State

- **current_day:** 6
- **last_completed:** Day 5 — Pointer Arithmetic (2026-05-03)
- **start_date:** 2026-04-29
- **lesson_format:** Concept → Code Example → **Quant Application** → Pitfalls → Exercise → Tomorrow's Preview

## Phase 1 — Foundations (Days 1–21) — `01-foundations/`

- [x] Day 1 — Compilation model: preprocessor, compiler, linker — [link](01-foundations/2026-04-29-day-01-compilation-model.md)
- [x] Day 2 — Fundamental types and sizes — [link](01-foundations/2026-04-29-day-02-fundamental-types-and-sizes.md)
- [x] Day 3 — `const`, `constexpr`, `mutable` — [link](01-foundations/2026-04-30-day-03-const-constexpr-mutable.md)
- [x] Day 4 — References vs pointers — [link](01-foundations/2026-05-01-day-04-references-vs-pointers.md)
- [x] Day 5 — Pointer arithmetic — [link](01-foundations/2026-05-03-day-05-pointer-arithmetic.md)
- [ ] Day 6 — Stack vs heap basics
- [ ] Day 7 — Functions: pass by value, ref, const ref
- [ ] Day 8 — Function overloading
- [ ] Day 9 — Default arguments
- [ ] Day 10 — Header files, include guards, `#pragma once`
- [ ] Day 11 — Translation units, ODR (One Definition Rule)
- [ ] Day 12 — `static` and `extern` linkage
- [ ] Day 13 — Namespaces
- [ ] Day 14 — C-style arrays
- [ ] Day 15 — `std::array`
- [ ] Day 16 — `std::string` vs C-strings
- [ ] Day 17 — `std::string_view`
- [ ] Day 18 — iostream basics, formatting
- [ ] Day 19 — Error handling: `errno`, `assert`, exceptions overview
- [ ] Day 20 — Compiler flags & optimization levels (-O0..-O3, -march)
- [ ] Day 21 — Debugging with `gdb`/`lldb`

## Phase 2 — OOP & Memory (Days 22–42) — `02-oop-memory/`

- [ ] Day 22 — `class` vs `struct`
- [ ] Day 23 — Constructors and member initializer lists
- [ ] Day 24 — Destructors and RAII
- [ ] Day 25 — Copy constructor and copy assignment
- [ ] Day 26 — Move constructor and move assignment
- [ ] Day 27 — Rule of 0 / 3 / 5
- [ ] Day 28 — `const` member functions
- [ ] Day 29 — `static` members
- [ ] Day 30 — `friend` functions and classes
- [ ] Day 31 — Inheritance basics
- [ ] Day 32 — Virtual functions and the vtable
- [ ] Day 33 — Pure virtual and abstract classes
- [ ] Day 34 — Multiple inheritance pitfalls
- [ ] Day 35 — Polymorphism in practice
- [ ] Day 36 — Stack vs heap deep dive
- [ ] Day 37 — `new`/`delete` and why to avoid raw
- [ ] Day 38 — `std::unique_ptr`
- [ ] Day 39 — `std::shared_ptr` and reference counting
- [ ] Day 40 — `std::weak_ptr` and circular references
- [ ] Day 41 — Custom deleters
- [ ] Day 42 — Memory leaks and detection (valgrind, ASan)

## Phase 3 — Modern C++ Core (Days 43–63) — `03-modern-cpp/`

- [ ] Day 43 — `auto` and type deduction
- [ ] Day 44 — `decltype`
- [ ] Day 45 — Range-based `for`
- [ ] Day 46 — Lambdas: basics and captures
- [ ] Day 47 — Lambdas: generic, mutable, init capture
- [ ] Day 48 — `std::function` vs raw lambdas (perf)
- [ ] Day 49 — `nullptr` vs `NULL`
- [ ] Day 50 — `enum class`
- [ ] Day 51 — `constexpr` functions
- [ ] Day 52 — `consteval` (C++20)
- [ ] Day 53 — `noexcept` and exception specifications
- [ ] Day 54 — `std::optional`
- [ ] Day 55 — `std::variant` and `std::visit`
- [ ] Day 56 — `std::tuple` and structured bindings
- [ ] Day 57 — `std::any`
- [ ] Day 58 — Uniform initialization `{}`
- [ ] Day 59 — Delegating constructors
- [ ] Day 60 — `final` and `override`
- [ ] Day 61 — Trailing return types
- [ ] Day 62 — `if constexpr`
- [ ] Day 63 — Aggregate initialization

## Phase 4 — Templates & STL (Days 64–91) — `04-templates-stl/`

- [ ] Day 64 — Function templates
- [ ] Day 65 — Class templates
- [ ] Day 66 — Template specialization
- [ ] Day 67 — Variadic templates
- [ ] Day 68 — SFINAE intro
- [ ] Day 69 — Concepts (C++20)
- [ ] Day 70 — Type traits
- [ ] Day 71 — CRTP pattern
- [ ] Day 72 — Iterator categories
- [ ] Day 73 — `std::vector` deep dive
- [ ] Day 74 — `std::array` vs `std::vector`
- [ ] Day 75 — `std::deque`
- [ ] Day 76 — `std::list` and `std::forward_list`
- [ ] Day 77 — `std::map` vs `std::unordered_map`
- [ ] Day 78 — `std::set`, `std::unordered_set`
- [ ] Day 79 — `std::multimap`, `std::multiset`
- [ ] Day 80 — `std::priority_queue` (heap)
- [ ] Day 81 — `std::stack` and `std::queue` adapters
- [ ] Day 82 — Algorithms: `sort`, `find`, `transform`
- [ ] Day 83 — Algorithms: `accumulate`, `reduce`, `partial_sum`
- [ ] Day 84 — Algorithms: `partition`, `nth_element`
- [ ] Day 85 — `std::ranges` (C++20)
- [ ] Day 86 — Custom comparators
- [ ] Day 87 — Allocators basics
- [ ] Day 88 — Move iterators
- [ ] Day 89 — Reverse iterators
- [ ] Day 90 — Custom iterators
- [ ] Day 91 — Iterator invalidation rules

## Phase 5 — Concurrency (Days 92–112) — `05-concurrency/`

- [ ] Day 92 — `std::thread` basics
- [ ] Day 93 — Joining and detaching
- [ ] Day 94 — Race conditions
- [ ] Day 95 — `std::mutex` and `lock_guard`
- [ ] Day 96 — `unique_lock` and `scoped_lock`
- [ ] Day 97 — `std::shared_mutex` (reader-writer)
- [ ] Day 98 — Deadlocks and prevention
- [ ] Day 99 — `std::condition_variable`
- [ ] Day 100 — `std::atomic` basics
- [ ] Day 101 — Memory orders (relaxed, acquire, release, seq_cst)
- [ ] Day 102 — Compare-and-swap (CAS)
- [ ] Day 103 — `std::async` and `std::future`
- [ ] Day 104 — `std::promise`
- [ ] Day 105 — `std::packaged_task`
- [ ] Day 106 — Thread pools
- [ ] Day 107 — Lock-free queue intro
- [ ] Day 108 — SPSC ring buffer
- [ ] Day 109 — False sharing
- [ ] Day 110 — Cache coherence basics (MESI)
- [ ] Day 111 — `std::jthread` (C++20)
- [ ] Day 112 — Coroutines intro (C++20)

## Phase 6 — Performance (Days 113–133) — `06-performance/`

- [ ] Day 113 — CPU cache hierarchy
- [ ] Day 114 — Cache lines and alignment
- [ ] Day 115 — Data-oriented design
- [ ] Day 116 — Branch prediction
- [ ] Day 117 — `[[likely]]` / `[[unlikely]]`
- [ ] Day 118 — SIMD basics: SSE, AVX
- [ ] Day 119 — `std::execution` policies (parallel STL)
- [ ] Day 120 — Custom memory pools
- [ ] Day 121 — Stack allocators
- [ ] Day 122 — Object pools
- [ ] Day 123 — Move semantics deep dive
- [ ] Day 124 — RVO / NRVO
- [ ] Day 125 — Copy elision
- [ ] Day 126 — Inlining and `__forceinline`
- [ ] Day 127 — Profile-guided optimization (PGO)
- [ ] Day 128 — Google Benchmark
- [ ] Day 129 — `perf` and profiling
- [ ] Day 130 — Latency measurement (RDTSC, `std::chrono`)
- [ ] Day 131 — Compile-time vs runtime tradeoffs
- [ ] Day 132 — Template metaprogramming for performance
- [ ] Day 133 — Expression templates

## Phase 7 — Quant-Specific (Days 134–168) — `07-quant-specific/`

- [ ] Day 134 — Order book: flat array vs map
- [ ] Day 135 — Order book: limit-level structures
- [ ] Day 136 — Price ladder optimization
- [ ] Day 137 — Limit order book matching engine
- [ ] Day 138 — Time series storage: circular buffers
- [ ] Day 139 — Tick data structures
- [ ] Day 140 — Fixed-point arithmetic vs `double`
- [ ] Day 141 — Decimal class for prices
- [ ] Day 142 — FIX protocol parsing
- [ ] Day 143 — Binary protocol parsing (ITCH, OUCH)
- [ ] Day 144 — Boost.Asio: TCP basics
- [ ] Day 145 — Boost.Asio: async I/O
- [ ] Day 146 — UDP multicast for market data
- [ ] Day 147 — Kernel bypass intro (DPDK, Solarflare)
- [ ] Day 148 — Lock-free order matching
- [ ] Day 149 — Eigen for risk matrices
- [ ] Day 150 — Numerical stability
- [ ] Day 151 — RNGs (Mersenne Twister, PCG)
- [ ] Day 152 — Monte Carlo patterns
- [ ] Day 153 — Black-Scholes implementation
- [ ] Day 154 — Greeks computation
- [ ] Day 155 — Portfolio optimization basics
- [ ] Day 156 — Risk metrics (VaR, expected shortfall)
- [ ] Day 157 — Backtesting frameworks
- [ ] Day 158 — Strategy harness design
- [ ] Day 159 — Latency budget breakdown
- [ ] Day 160 — Hot/cold path separation
- [ ] Day 161 — Pre-allocation patterns
- [ ] Day 162 — Static polymorphism (templates) vs virtual
- [ ] Day 163 — Type-erased callbacks vs templates
- [ ] Day 164 — HFT patterns: single-threaded core
- [ ] Day 165 — HFT patterns: thread pinning, NUMA
- [ ] Day 166 — Production C++: logging, monitoring
- [ ] Day 167 — Code review for HFT
- [ ] Day 168 — Capstone: mini matching engine

## Completed Lessons Log

- Day 1 — Compilation model — [link](01-foundations/2026-04-29-day-01-compilation-model.md)
- Day 2 — Fundamental types and sizes — [link](01-foundations/2026-04-29-day-02-fundamental-types-and-sizes.md)
- Day 3 — `const`, `constexpr`, `mutable` — [link](01-foundations/2026-04-30-day-03-const-constexpr-mutable.md)
- Day 4 — References vs Pointers — [link](01-foundations/2026-05-01-day-04-references-vs-pointers.md)
- Day 5 — Pointer Arithmetic — [link](01-foundations/2026-05-03-day-05-pointer-arithmetic.md)
