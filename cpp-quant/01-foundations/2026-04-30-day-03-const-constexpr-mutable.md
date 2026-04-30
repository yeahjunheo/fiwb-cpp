---
date: 2026-04-30
day: 3
phase: 01-foundations
tags: [cpp, quant, const, constexpr, mutable]
---

# Day 3 — `const`, `constexpr`, and `mutable`

> **Why this matters:** You now know the types that hold your data (Day 2). The next step is learning how to make certain values *read-only* — and, crucially, *when* that protection is enforced. C++ gives you three related tools: `const` says "I promise this won't change once set," `constexpr` says "this must be known at compile time," and `mutable` carves a narrow exception to const inside classes. These keywords appear in nearly every line of professional C++ code, so building a clear mental model now pays off through the entire curriculum.

## Concept

When you write a trading strategy, some values are fixed truths of the world: the minimum price increment for a US equity, the number of nanoseconds in a second, the commission rate your broker charges. Accidentally changing one of those mid-run is a bug, not a feature. C++ lets you encode that intent directly: mark a variable unchangeable, and the *compiler* will refuse to build any code that tries to modify it. You catch entire classes of bugs before the program ever runs.

### `const` — a promise: "this value won't change once set"

Declaring a variable `const` tells the compiler two things: (1) the variable must be initialized immediately — you cannot declare it and fill it in later; (2) nothing in the program may reassign it after that initialization point.

```
const double commission_rate = 0.0025;   // 0.25% per trade — set once, never changed
commission_rate = 0.003;                 // COMPILE ERROR: assignment of read-only variable
```

A key nuance beginners often miss: the value of a `const` variable does **not** have to be known at compile time. It simply has to be provided at the moment the variable is created. This is completely legal:

```cpp
// Suppose the user types in a threshold when the program starts.
// const double threshold = read_from_config_file();   // value known only at runtime
// That's fine — const just means "don't change it after this point."
```

The compiler enforces the "no changes afterward" rule; it does not require the value to be baked into the binary at build time. That stronger requirement belongs to `constexpr`.

### `constexpr` — a stronger promise: "this must be known at compile time"

`constexpr` (short for "constant expression") adds one more constraint on top of const: the initializer must be computable by the compiler during the build, before the program runs at all. Try to initialize a `constexpr` variable from a runtime value and the compiler refuses.

```cpp
constexpr int    MAX_ORDERS  = 10000;              // ok — literal, known at compile time
constexpr double TICK_SIZE   = 0.01;               // ok
constexpr int    BYTES_IN_64 = sizeof(int64_t);    // ok — sizeof is a compile-time operator
// constexpr int limit = some_runtime_call();      // COMPILE ERROR — not a constant expression
```

`constexpr` implies `const` automatically. A `constexpr` variable is always const. The reverse is not true: a plain `const` variable is **not** automatically `constexpr`.

Why does the compile-time distinction matter? Two reasons.

**Correctness.** Some C++ features require a value the compiler knows during the build — for example, the size of a C-style array (which you'll meet on Day 14) must be a compile-time constant. `constexpr` is the unambiguous way to satisfy that requirement. A plain `const int` can also work for integral types with a literal initializer, but `constexpr` makes the intent clear and portable, and it works for all types including `double`.

**Performance.** The compiler can fold a `constexpr` value directly into the generated machine instructions rather than allocating memory for it. In latency-sensitive code that runs millions of times per second — the kind you'll write in Phase 6 — eliminating even a handful of memory reads per iteration can reduce processing time from microseconds to nanoseconds.

> **Key idea:** Use `constexpr` when the value is a fixed mathematical or structural constant that logically belongs in the build — tick sizes, buffer capacities, loop bounds, conversion factors. Use `const` when the value is fixed for its lifetime but is only available at runtime — a threshold read from a config file, a cap computed from user input.

### `mutable` — the narrow exception to const

`mutable` only applies to *class member variables*. A **class** (a user-defined type that groups data and functions — coming on Day 22) can be marked `const` as a whole, making all its data read-only. But a member explicitly tagged `mutable` can still be modified even then.

This sounds like a contradiction, but there is a clean use case: data that is implementation bookkeeping rather than part of the object's logical value. Think of a read-only order object in a trading system: its price, quantity, and side are logically fixed. But if you want to track how many times the order was inspected for risk purposes, that counter does not change *what the order is* — it is internal housekeeping. Marking it `mutable` lets a `const` "read" operation update the counter without breaking the const contract.

Because `mutable` requires classes, a full compilable example has to wait until Day 22. For now, the mental model to hold is: `mutable` punches a deliberate hole in `const` for members whose state is bookkeeping rather than identity.

> **Note:** In Phase 1 you will use `const` and `constexpr` constantly (on variables, soon on function parameters). You will encounter `mutable` rarely. Learn the concept now so it doesn't surprise you later; the exercise today covers only `const` and `constexpr`.

## Code Example

Create `constants.cpp`:

```cpp
// constants.cpp
// Explore const, constexpr, and the difference between them.

#include <iostream>
#include <cstdint>      // int32_t, int64_t

int main() {
    // --- constexpr: value MUST be determined at compile time ---
    constexpr int32_t MAX_ORDER_QTY = 10000;          // max shares per single order
    constexpr double  TICK_SIZE     = 0.01;            // smallest price increment, US equities
    constexpr int64_t NS_PER_SECOND = 1'000'000'000LL; // nanoseconds in one second
    constexpr int32_t TIMESTAMP_BYTES = sizeof(int64_t); // sizeof is a compile-time operator

    std::cout << "Max order qty   : " << MAX_ORDER_QTY    << "\n";
    std::cout << "Tick size       : " << TICK_SIZE         << "\n";
    std::cout << "Nanoseconds/sec : " << NS_PER_SECOND     << "\n";
    std::cout << "Timestamp bytes : " << TIMESTAMP_BYTES   << "\n";

    // --- const: value fixed once set, but may come from a runtime expression ---
    int32_t base_qty = 200;          // ordinary mutable variable
    base_qty = base_qty * 3;         // runtime modification — perfectly fine
    const int32_t session_cap = base_qty;  // locked in from this point on
    // session_cap = 999;            // would be a COMPILE ERROR if uncommented
    std::cout << "Session cap     : " << session_cap << "\n";

    // --- constexpr is implicitly const: this line would also fail ---
    // TICK_SIZE = 0.02;             // COMPILE ERROR if uncommented

    return 0;
}
```

Compile and run:

```bash
g++ -std=c++17 -Wall constants.cpp -o constants
./constants
```

Expected output:

```
Max order qty   : 10000
Tick size       : 0.01
Nanoseconds/sec : 1000000000
Timestamp bytes : 8
Session cap     : 600
```

Two things to notice.

The digit separator `1'000'000'000LL` — C++14 and later allow single quotes inside number literals as a visual separator. They are ignored by the compiler; they just make large numbers readable. The `LL` suffix means "treat this as a `long long` literal" (a 64-bit integer), ensuring the number isn't truncated before it is stored in `NS_PER_SECOND`.

The `session_cap` example shows that `const` accepts a runtime value (`base_qty` after modification). If you needed `session_cap` in a context requiring a compile-time constant — as an array size, for example — this would fail at compile time because the compiler cannot know `base_qty * 3` during the build. That is the moment you need `constexpr`.

**Seeing the error yourself.** Add this line inside `main` before `return 0`, recompile, then remove it:

```cpp
TICK_SIZE = 0.05;   // deliberate error
```

You should see:
```
constants.cpp: error: assignment of read-only variable 'TICK_SIZE'
```

The compiler caught a modification to a value that should never change — exactly as intended.

## Quant Application

Trading systems are full of fixed parameters: the minimum price increment (**tick size** — the smallest allowed price move for a security), the maximum position size an algorithm is allowed to hold, the number of decimal places in a price, conversion constants for timestamps. Declaring these `constexpr` is not just a style choice — it gives the compiler license to embed them directly in the generated machine code rather than loading them from memory at runtime. In a loop that processes tens of millions of price updates per second, eliminating memory loads matters. Each memory read that hits a cache miss (fast memory that the CPU uses to avoid going all the way to RAM — more on this in Phase 6) can cost hundreds of nanoseconds. Constants baked in at compile time never touch memory at all.

`const` plays an equally important role at the design level. When every variable and function parameter that should not change is marked `const`, the handful of places where data *actually* mutates stand out immediately. Quant developers call this discipline **const-correctness**: a systematic habit of marking everything that should be read-only as `const`, so code reviewers and future maintainers can see mutation sites at a glance. In a risk-management system where a misplaced write to a position counter can cause a firm to exceed its regulatory limits, this kind of clarity is not optional — it is how teams audit their code before something goes wrong. You will see `const` on almost every parameter in Phase 2+ examples.

`mutable` exists because some objects have two layers of state: a logical layer that defines what the object *is* (an order's price and quantity) and an implementation layer that is internal bookkeeping (how many times it was read, whether a cached summary needs recomputing). Marking bookkeeping members `mutable` lets the object present a `const` face to the outside world while still tracking its own internal metadata. You will encounter `mutable` most often in connection with mutexes (locks used to protect data in a multi-threaded program — Day 95) and cached computations (storing a precomputed result so it doesn't have to be recomputed each time).

## Pitfalls

- **Declaring `const` without initializing.** `const int x;` is a compile error. A `const` variable has no "default value" — you must provide the value right at the declaration. There is no way to fill it in on the next line.

- **Expecting `const` to guarantee compile-time evaluation.** `const int N = 10;` can work as an array size for integral types, but `const double rate = 0.01;` cannot — it is not a compile-time constant. And `const int M = some_function();` cannot be used as an array size even though it looks similar. If you need a guaranteed compile-time constant, write `constexpr`. The intent is unambiguous and works for all types.

- **Trying to initialize `constexpr` from a runtime value.** `constexpr int limit = get_from_config();` will not compile if `get_from_config()` is an ordinary (non-constexpr) function. Use `const` instead when the value is only available at runtime.

- **Forgetting `constexpr` implies `const`.** There is no such thing as a non-const `constexpr` variable. Attempting to assign to a `constexpr` variable is the same error as assigning to a `const` variable.

- **`mutable` outside a class.** Putting `mutable` on a local variable — `mutable int x = 5;` — is a compile error. `mutable` is only valid on non-static class member variables. You won't need this until Day 22; just don't be surprised if a compiler error mentions it.

- **Using `constexpr` without an initializer.** `constexpr int x;` is a compile error for the same reason as `const int x;`: there is no value to make constant. Always provide the initializer on the same line.

## Exercise

Goal: experience the `const` vs `constexpr` distinction by producing both the working version and the intentional failure.

1. Create a new folder and a file `day03.cpp`. Add `#include <iostream>` and `#include <cstdint>` at the top. Write a `main()` that returns `0`.

2. Inside `main`, declare these four variables and print each one with `std::cout`:
   ```cpp
   constexpr double  TICK        = 0.01;
   constexpr int64_t NS_PER_MS   = 1'000'000LL;  // nanoseconds in one millisecond
   const     double  spread      = 2.0 * TICK;    // derived from a constexpr — legal
   int32_t           qty         = 200;            // mutable
   ```
   Compile and run:
   ```bash
   g++ -std=c++17 -Wall day03.cpp -o day03
   ./day03
   ```
   Confirm output includes `0.01`, `1000000`, `0.02`, and `200`.

3. Mutate the mutable one: add `qty = qty * 3;` and re-print `qty`. Recompile and confirm `qty` is now `600`. This is the baseline that works.

4. Now add `TICK = 0.05;` on the next line. Try to recompile. Read the error carefully — it will tell you which variable is read-only and on which line you tried to change it. Then remove that line and confirm the build is clean again.

5. Replace `const double spread = 2.0 * TICK;` with `int32_t runtime_val = qty; const int32_t locked = runtime_val;`. Print `locked`. Compile and run. Now try adding `locked = locked + 1;` — this should fail with the same read-only error. Remove the failing line.

6. Self-check: answer these before moving on. "Why can `spread` be initialized from `TICK` even though `spread` is only `const`?" (Because `TICK` is `constexpr` — it is a valid constant expression, so `2.0 * TICK` is also a constant expression. A `const` variable can be initialized from any value, including compile-time ones.) "Could I declare `spread` as `constexpr` too?" (Yes — `constexpr double spread = 2.0 * TICK;` is valid because the initializer is a constant expression.)

## Glossary

- **Tick size** — the minimum allowed price increment for a security (e.g., $0.01 for most US equities). Covered in context in Phase 7 (Day 139).
- **Class** — a user-defined type that bundles data and functions together. Covered from Day 22.
- **Const-correctness** — the practice of marking every variable and parameter that should not change as `const`, so that mutation points stand out during code review.
- **Constexpr function** — a function that can be evaluated at compile time when all its arguments are compile-time constants. Covered on Day 51.
- **Cache miss** — what happens when the CPU needs data that is not in its fast local memory and must fetch it from slower RAM. Covered in depth on Day 113.

## Tomorrow's Preview

**Day 4 — References and Pointers.** A reference is an alias — another name for an existing variable. A pointer holds a variable's memory address. Both interact directly with `const`: you'll immediately encounter `const int&` and `const int*`, and today's lesson is exactly what you need to read those types fluently.

## Links

- [Roadmap](../cpp-roadmap.md)
- Previous: [Day 2 — Fundamental Types and Sizes](2026-04-29-day-02-fundamental-types-and-sizes.md)
