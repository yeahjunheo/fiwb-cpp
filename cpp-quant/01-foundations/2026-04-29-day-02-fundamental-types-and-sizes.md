---
date: 2026-04-29
day: 2
phase: 01-foundations
tags: [cpp, quant, types, primitives]
---

# Day 2 — Fundamental Types and Sizes

> **Why this matters:** Yesterday you saw how a `.cpp` file becomes an executable. Today you meet the alphabet that file is written in: the built-in types. C++ has many number types where Python has one, and each one has a fixed size in memory. Knowing which type to pick — and why — is the first real C++ skill, and it shows up everywhere from "why did my number wrap around?" to "why does this trading system use `int64_t` for timestamps?"

## Concept

In Python you write `x = 5` and never think about whether it's an integer or a float or how many bytes it takes. C++ makes you say it. Every variable has a **type** declared up front, and that type tells the compiler exactly how many bytes to set aside and how to interpret those bytes. This is the price you pay for speed: the compiler does no guessing at runtime.

Think of memory as a long row of mailbox slots. When you write `int x = 5;`, you are reserving a fixed-size cluster of slots (usually 4 of them) and writing the bit pattern for "5" into them. The label `int` tells the compiler "these slots, together, mean a whole number." If you instead write `double y = 5.0;`, you reserve 8 slots and the compiler interprets them with a different scheme (one designed for fractional numbers). Same underlying memory, different *meaning*, all decided at compile time.

There are three families to know on Day 2.

**Integer types** hold whole numbers. The main ones are `char` (1 byte, ~ -128 to 127), `short` (usually 2 bytes), `int` (usually 4 bytes, the everyday workhorse), `long` (4 or 8 bytes depending on platform — annoyingly inconsistent), and `long long` (at least 8 bytes). Each can be `signed` (default, can be negative) or `unsigned` (only zero and positive, but reaches twice as high). Because the size of `int` and `long` can vary across compilers, C++ also gives you fixed-width names from the `<cstdint>` header: `int32_t`, `int64_t`, `uint64_t`, etc. Use those when the size matters.

**Floating-point types** hold numbers with a fractional part: `float` (4 bytes, ~7 decimal digits of precision), `double` (8 bytes, ~15 digits, the everyday workhorse), and `long double` (platform-dependent, often 16 bytes). They can store huge or tiny magnitudes but with limited precision — not every decimal can be represented exactly. We'll dig into the consequences later; for now, just know "double is precise enough for most things, but is not exact."

**Other essentials.** `bool` is true/false (technically 1 byte even though it only needs 1 bit). `char` doubles as both a tiny integer and a single character (`'A'` is the integer 65 with a `char` label). Finally, `size_t` is an unsigned integer type used for sizes and counts; it shows up everywhere in the standard library, so you'll see it constantly.

> **Key idea:** `sizeof(T)` tells you how many bytes type `T` takes on your machine. It is computed at compile time. `sizeof(int)` is almost always 4. `sizeof(double)` is almost always 8. When in doubt, ask the compiler — print it.

> **Warning: Integer overflow is silent.** If you store 2,200,000,000 in a 32-bit `int` (which maxes out around 2.14 billion), the value silently wraps around to a negative number. No error, no warning — just wrong math. This is one of the most common bug sources in C++, and we will respect it.

> **Note: Why so many types?** Memory and speed. A market-data feed sending billions of price updates a day saves real RAM and bandwidth when each price uses 4 bytes instead of 8. And the CPU operates faster on data that fits its native word size. C++ exposes the choice; Python hides it.

## Code Example

Two snippets. Type each into a single file `types.cpp` (the second is a separate file `overflow.cpp`).

```cpp
// types.cpp
// Show the size and range of common fundamental types.

#include <iostream>   // for std::cout
#include <climits>    // for INT_MAX, INT_MIN, etc. (limits of integer types)
#include <cstdint>    // for int32_t, int64_t, etc.

int main() {
    // sizeof(T) returns the number of bytes type T occupies.
    std::cout << "sizeof(char):      " << sizeof(char)      << "\n";
    std::cout << "sizeof(short):     " << sizeof(short)     << "\n";
    std::cout << "sizeof(int):       " << sizeof(int)       << "\n";
    std::cout << "sizeof(long):      " << sizeof(long)      << "\n";
    std::cout << "sizeof(long long): " << sizeof(long long) << "\n";
    std::cout << "sizeof(float):     " << sizeof(float)     << "\n";
    std::cout << "sizeof(double):    " << sizeof(double)    << "\n";
    std::cout << "sizeof(bool):      " << sizeof(bool)      << "\n";

    // Range of int on this machine.
    std::cout << "\nint goes from " << INT_MIN << " to " << INT_MAX << "\n";

    // Fixed-width types: same size everywhere.
    int32_t a = 100;     // exactly 32 bits, signed
    uint64_t b = 100;    // exactly 64 bits, unsigned
    std::cout << "int32_t a = " << a << ", uint64_t b = " << b << "\n";

    return 0;
}
```

Compile and run:

```bash
g++ -std=c++17 -Wall types.cpp -o types
./types
```

Expected output (on most 64-bit Linux/Mac systems):

```
sizeof(char):      1
sizeof(short):     2
sizeof(int):       4
sizeof(long):      8
sizeof(long long): 8
sizeof(float):     4
sizeof(double):    8
sizeof(bool):      1

int goes from -2147483648 to 2147483647
int32_t a = 100, uint64_t b = 100
```

Now overflow:

```cpp
// overflow.cpp
// Watch a 32-bit int wrap around when pushed past its limit.

#include <iostream>
#include <climits>

int main() {
    int x = INT_MAX;       // largest value an int can hold (2,147,483,647)
    std::cout << "x        = " << x     << "\n";
    std::cout << "x + 1    = " << x + 1 << "\n";   // wraps to a large negative number

    unsigned int u = 0;
    std::cout << "u        = " << u     << "\n";
    std::cout << "u - 1    = " << u - 1 << "\n";   // wraps the OTHER way: huge positive number

    return 0;
}
```

Compile and run:

```bash
g++ -std=c++17 -Wall overflow.cpp -o overflow
./overflow
```

Expected output:

```
x        = 2147483647
x + 1    = -2147483648
u        = 0
u - 1    = 4294967295
```

Notice that `x + 1` did not error — it silently became the most negative `int`. And subtracting from an `unsigned int` zero gave you the biggest possible `unsigned int`. The bits underneath wrapped around like an odometer.

## Quant Application

Picking the right type is one of the most consequential choices in a trading system. Consider a **timestamp** — a number that records *when* something happened. Modern exchanges deliver **market data** (the live stream of price updates from an exchange) with nanosecond resolution. A 32-bit unsigned integer can count up to about 4.3 billion. There are a billion nanoseconds in a single second, so a 32-bit timestamp would overflow in roughly 4 seconds. That is why timestamps in real systems are always 64-bit (`int64_t` or `uint64_t`). Get this wrong and your system silently records "yesterday" as "next century" because the counter wrapped.

Prices are the second classic example. A naive instinct is to use `double` for everything, since prices look like decimals — `137.42`. But `double` cannot represent every decimal exactly (you'll see this concretely on Day 140 when we cover **fixed-point arithmetic** — using integers to represent prices in cents or smaller units to avoid rounding error). For now, the takeaway is: even at this level, *type choice carries financial meaning*. The wrong choice can cause an order to fill at $137.4199999 instead of $137.42, and over millions of orders that drift becomes real money. Many production systems store prices as 64-bit integers in units of 0.0001 (called "ticks" — a **tick** is a single price increment) precisely to sidestep `double` rounding.

Memory and cache also tie directly back to types. A 32-bit `int` takes half the space of a 64-bit `int`, which means twice as many fit in the CPU's fast cache (more on cache on Day 113). A trading firm processing millions of order book updates per second cares deeply about whether each record is 16 bytes or 32 bytes — it can mean the difference between a strategy that fits in cache and runs at microsecond latency, and one that doesn't and runs at millisecond latency. Day 1 you learned the build pipeline; today you learn that the *data alphabet* you choose has compound performance effects all the way down.

## Pitfalls

- **Assuming `int` is always 32 bits.** On most modern desktops it is, but the C++ standard only guarantees at least 16 bits. When the size matters (timestamps, file formats, network protocols), use `int32_t` or `int64_t` from `<cstdint>`.
- **Mixing signed and unsigned in comparisons.** `int i = -1; unsigned u = 1; if (i < u) { ... }` does not behave the way you'd expect — `i` gets converted to a huge unsigned number and the condition is false. Compile with `-Wall` and the compiler will warn you.
- **Using `int` for things that can grow large.** Counts of nanoseconds, byte sizes of large files, identifiers in big systems — all need 64-bit types. A 32-bit int caps at ~2.1 billion.
- **Reaching for `float` because it's "smaller and faster."** On modern CPUs `double` is the default for arithmetic and is often the same speed. Use `float` only when you have a specific reason (memory pressure, GPU code).
- **Believing `0.1 + 0.2 == 0.3` works with `double`.** It does not, because 0.1 is not exactly representable in binary floating-point. We will return to this; for now, don't compare floating-point numbers with `==`.
- **Forgetting that `char` is an integer.** Writing `char c = 65; std::cout << c;` prints `A`, not `65`. If you want the number, cast: `std::cout << static_cast<int>(c);` (the `static_cast<int>(c)` form just means "treat `c` as an `int` for this expression").

## Exercise

Goal: see types and overflow with your own eyes.

1. Create a fresh folder. Inside it, create a file called `sizes.cpp` and paste in the `types.cpp` snippet from the Code Example above.

2. From a terminal in that folder, compile and run:
   ```bash
   g++ -std=c++17 -Wall sizes.cpp -o sizes
   ./sizes
   ```
   You should see the sizes printed. Note any value that surprises you — `sizeof(long)` on Mac/Linux is 8, but on Windows MSVC it's 4. That's the inconsistency we warned about.

3. In the same folder, create `overflow.cpp` with the second snippet. Compile and run:
   ```bash
   g++ -std=c++17 -Wall overflow.cpp -o overflow
   ./overflow
   ```
   Confirm `x + 1` wraps to a negative number and `u - 1` wraps to ~4.29 billion.

4. **Modify** `overflow.cpp`: change `int x` to `int64_t x` and `INT_MAX` to `INT64_MAX`. Add `#include <cstdint>` if it isn't already. Recompile and rerun. Notice that now `x + 1` *also* overflows, but you had to push to a vastly larger number first (~9.2 quintillion). This is why timestamps use 64-bit types.

5. Self-check: you should be able to answer "how many bytes does a `double` take?" (8) and "what happens when an `int` exceeds `INT_MAX`?" (it silently wraps to a negative number). If `g++` says `INT_MAX was not declared in this scope`, you forgot `#include <climits>`.

## Glossary

- **Tick** — a single price increment on an exchange (e.g., one cent for many US stocks). Fully covered in Phase 7 (Day 139).
- **Fixed-point arithmetic** — using integers to represent fractional values (like prices in cents) to avoid floating-point rounding. Day 140.
- **Cache** — small, very fast memory the CPU uses to hold recently-touched data. Day 113.

## Tomorrow's Preview

**Day 3 — `const`, `constexpr`, `mutable`.** You'll learn how to mark variables as unchangeable, why that helps the compiler write faster code, and what "evaluated at compile time" really means. Builds on today: every `const` variable still has one of the types you met today; the new keywords just restrict how it can be used.

## Links

- [Roadmap](../cpp-roadmap.md)
- Previous: [Day 1 — Compilation Model](2026-04-29-day-01-compilation-model.md)
