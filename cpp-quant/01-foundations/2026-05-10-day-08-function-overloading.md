---
date: 2026-05-10
day: 8
phase: 01-foundations
tags: [cpp, quant, functions, overloading]
---

# Day 8 — Function Overloading

> **Why this matters:** Yesterday you learned that how a function declares its parameter — by value, by reference, or by const reference — determines whether it copies your data or shares it. Today you discover that C++ lets you give multiple functions the same name as long as their parameter lists differ. The compiler then picks the right version automatically based on what you pass in. This is called **function overloading**, and it shows up constantly in real C++ code.

## Concept

### The problem overloading solves

Imagine you want a function that prints a price. Sometimes the price arrives from the exchange as an integer (a number of whole cents, or whole "ticks"), and sometimes it's a `double` (a fractional price like 100.25). Without overloading, you need two differently named functions:

```cpp
void print_int_price(int32_t price) { ... }
void print_double_price(double price) { ... }
```

The caller has to remember which name goes with which type — an unnecessary burden that grows as the codebase gets larger. **Function overloading** lets you write both functions with the same name `print_price`, and the compiler chooses the right one automatically.

### What makes two functions valid overloads

Two functions are **overloads** of each other when they share the same name but differ in their **parameter list** — either the number of parameters, the types of parameters, or both:

```cpp
void print_price(int32_t price) { /* integer version */ }
void print_price(double price)  { /* floating-point version */ }
```

Both are named `print_price`. The compiler considers these two distinct functions because their parameter lists differ.

The full collection of functions sharing a name that are visible at a given call site is called the **overload set**. When you write `print_price(x)`, the compiler inspects every function in the overload set and picks the one whose parameter list best matches the type of `x`.

> **Key idea:** Overloads are told apart by their parameter lists — the types and number of parameters. The return type plays no role in distinguishing overloads.

### How the compiler picks: overload resolution

The technical name for "the compiler picks the right overload" is **overload resolution**. Here is what the compiler does, step by step:

1. **Collect candidates.** Find every visible function with the matching name.
2. **Filter for viable ones.** Keep only functions whose parameters can accept your arguments, either directly or via an implicit conversion (a conversion the compiler does automatically, without you writing a cast).
3. **Rank by conversion quality.** Prefer the function that requires the least work to match your argument types.
4. **Pick the winner.** If exactly one function ranks best on every argument, that function is called. If two functions tie for best match, the call is **ambiguous** and the compiler reports an error — it refuses to guess.

### Conversion ranking

The compiler ranks how good a match is by the kind of conversion required:

| Rank (best to worst) | Description | Example |
|---|---|---|
| Exact match | No conversion at all | `int32_t` argument → `int32_t` parameter |
| Promotion | Small widening conversion | `float` argument → `double` parameter |
| Standard conversion | Any other implicit conversion | `double` argument → `int32_t` parameter (truncates) |

The compiler always prefers an exact match over a promotion, and a promotion over a standard conversion. This ranking determines which overload wins when an argument's type is not identical to any parameter type.

A call becomes ambiguous when two overloads require conversions of equal rank. For example, if you have `void f(float x)` and `void f(double x)` and you call `f(42)`, the compiler must convert the `int` literal `42` to either `float` or `double` — both require the same kind of conversion, so neither overload wins and the compiler reports an error.

### Overloading on the number of parameters

You can also overload by using a different *number* of parameters — same types, just more or fewer of them:

```cpp
double compute_spread(double bid, double ask) {
    return ask - bid;
}

double compute_spread(double bid, double ask, double commission) {
    return ask - bid + commission;
}
```

`compute_spread(99.5, 100.25)` calls the two-argument version. `compute_spread(99.5, 100.25, 0.05)` calls the three-argument version. The argument count is enough to resolve the call unambiguously.

### What you cannot overload on: return type

You cannot create valid overloads that differ **only** in return type. These two declarations are a re-declaration error:

```cpp
int32_t  get_quantity() { return 10; }
double   get_quantity() { return 10.0; }  // ERROR: same parameter list
```

The compiler cannot resolve which version to call from `get_quantity()` alone because the call site doesn't reveal the intended return type. The parameter list must differ.

### `const&` versus `&` are distinct overloads

From Day 7, you know that `int32_t&` and `const int32_t&` are genuinely different parameter types. That difference carries through to overloading:

```cpp
void inspect(int32_t& x)       { /* can modify x */ }
void inspect(const int32_t& x) { /* read-only x */ }
```

These are two distinct overloads. When you pass a regular (non-const) variable, the compiler prefers the non-const reference overload. When you pass a `const` variable or a temporary value, the compiler picks the `const` reference overload — because a mutable reference cannot bind to a `const` value (you saw this in Day 7).

Note that `const` on a **by-value** parameter does *not* create a distinct overload. `void f(int x)` and `void f(const int x)` look different on paper but are treated as the same declaration by the compiler, because `const` on a copy has no effect on the caller's side.

## Code Example

Create `20260510/day08.cpp`:

```cpp
// day08.cpp — function overloading: same name, different parameter lists

#include <iostream>
#include <cstdint>

// Overload 1: print an integer price (e.g., price in whole cents)
void print_price(int32_t price) {
    std::cout << "int  price: " << price << " cents\n";
}

// Overload 2: print a floating-point price
void print_price(double price) {
    std::cout << "dbl  price: " << price << "\n";
}

// Overload A: spread without commission
double compute_spread(double bid, double ask) {
    return ask - bid;
}

// Overload B: spread including a round-trip commission cost
double compute_spread(double bid, double ask, double commission) {
    return ask - bid + commission;
}

int main() {
    // --- Exact match picks the int32_t overload ---
    int32_t ticks = 10025;        // 100.25 dollars expressed in cents
    print_price(ticks);           // calls print_price(int32_t)

    // --- Exact match picks the double overload ---
    double price = 100.25;
    print_price(price);           // calls print_price(double)

    // --- 3.14 is a double literal — exact match for double overload ---
    print_price(3.14);            // calls print_price(double)

    std::cout << "\n";

    // --- Two-argument overload ---
    double spread = compute_spread(99.75, 100.25);
    std::cout << "spread (no fee):   " << spread << "\n";

    // --- Three-argument overload ---
    double total = compute_spread(99.75, 100.25, 0.05);
    std::cout << "spread (with fee): " << total << "\n";

    return 0;
}
```

Compile and run:

```bash
g++ -std=c++17 -Wall -Wextra day08.cpp -o day08
./day08
```

Expected output:

```
int  price: 10025 cents
dbl  price: 100.25
dbl  price: 3.14

spread (no fee):   0.5
spread (with fee): 0.55
```

Notice that `print_price(ticks)` — where `ticks` is `int32_t` — picks the integer overload without any instruction from you. `print_price(price)` and `print_price(3.14)` both pick the `double` overload. The selection is based entirely on the argument type at compile time, before your program ever runs.

## Quant Application

Exchanges — the electronic venues where buyers and sellers trade financial instruments like stocks and futures — often transmit prices as integers rather than floating-point numbers. This is done for two reasons: integers have perfectly exact representations (there is no floating-point rounding), and integer arithmetic is faster. A common encoding is to express prices in units of one ten-thousandth of a dollar: a price of \$100.25 becomes the integer 1002500. Inside a trading system, some modules work directly with these raw integer prices, while others normalize them to `double` for computation. An overloaded function like `format_price(int64_t raw)` and `format_price(double price)` lets any part of the system call the same logical operation regardless of which encoding it holds, keeping the calling code clean and readable.

In an order book — the live list of resting buy orders (bids) and sell orders (asks) for a security — you often need utility functions that accept either a price as a raw integer tick or as a `double`. For example, a function that checks whether a given price improves the current best bid could be written as two overloads: one for when you have just parsed a raw integer from the wire, and one for when you are working with normalized doubles after applying a price scale factor. The compiler automatically routes the call to the right overload based on what you have at hand, eliminating the manual type-checking logic that clutters code when functions have different names.

The critical performance note: overload resolution happens entirely at **compile time**. By the time your program runs, the compiler has already decided which function to call and has wired in the direct call. There is no runtime overhead of "picking" the right version the way you might imagine — no table lookup, no branching. In HFT (high-frequency trading — trading where execution latency is measured in single-digit microseconds and every nanosecond saved has value), overloaded functions are as fast as if you had called the unique-named version directly. This makes overloading a zero-cost abstraction: it improves code clarity without sacrificing speed.

## Pitfalls

- **Trying to overload on return type alone.** If you write `int get_price()` and `double get_price()` with no parameter differences, the compiler emits a re-declaration error. There is no way to resolve the ambiguity at the call site — the compiler does not look at how you use the return value when selecting overloads.

- **Creating an ambiguous call without realizing it.** If you have `void f(float x)` and `void f(double x)` and call `f(42)`, both conversions from `int` are equally ranked and the compiler reports an error: "call of overloaded 'f(int)' is ambiguous." The fix is to cast explicitly: `f(static_cast<double>(42))` or `f(42.0)`.

- **Thinking `f(int)` and `f(const int)` are different overloads.** For by-value parameters, top-level `const` does not distinguish overloads — the compiler treats `void f(int x)` and `void f(const int x)` as the same declaration and will reject the second as a duplicate. This is a common surprise. The rule only applies to by-value; `void f(int&)` and `void f(const int&)` are genuinely distinct.

- **Accidentally hiding outer-scope overloads.** If you declare a function with the same name inside a smaller scope (inside a block or inside a new function), it hides all the outer-scope overloads — even the ones with different parameter types. The inner declaration becomes the only candidate. This is rarely what you want; if you hit this, look for a local declaration shadowing a broader one.

- **Unexpected conversion picking the wrong overload.** You have `void log(int32_t x)` and `void log(double x)`. You call `log(counter)` where `counter` is `int64_t`. The compiler needs to convert `int64_t` to either `int32_t` or `double`. Both are standard conversions at the same rank, making the call ambiguous. When this happens, cast the argument explicitly: `log(static_cast<double>(counter))`.

- **Expecting overload selection to see the future.** The compiler resolves the overload based on the types of your arguments at the call site, and only those types — it does not consider what you intend to do with the return value, what variable you plan to store it in, or any surrounding context. If the argument types alone are ambiguous, the call fails.

## Exercise

Goal: write a set of overloads, call them with various argument types, and observe which version the compiler selects.

1. Create `20260510/day08_ex.cpp`. Add `#include <iostream>` and `#include <cstdint>`. Write these two overloads before `main`:
   - `void display(int32_t x)` — prints `"int: "` followed by `x` and a newline.
   - `void display(double x)` — prints `"dbl: "` followed by `x` and a newline.

2. Write two more overloads of `double total_value`:
   - `double total_value(double price, int32_t qty)` — returns `price * qty`.
   - `double total_value(double price, int32_t qty, double fee)` — returns `price * qty + fee`.

3. In `main`, call `display` with `int32_t count = 7;` and then with `double rate = 2.5;`. Before running, predict which overload each call will hit. Then call `display(3.14)` — predict which overload that hits too.

4. Call `total_value(100.0, 5)` and `total_value(100.0, 5, 1.25)`. Print both results. Predict what each will be before running.

5. Compile and run:
   ```bash
   g++ -std=c++17 -Wall -Wextra day08_ex.cpp -o day08_ex
   ./day08_ex
   ```
   Self-check: `display(count)` should print `int: 7`; `display(rate)` and `display(3.14)` should both print `dbl:`; `total_value(100.0, 5)` should give `500`; `total_value(100.0, 5, 1.25)` should give `501.25`. If the compiler rejects any call, read the error — it will name the argument type and list the candidates.

## Tomorrow's Preview

**Day 9 — Default Arguments.** You have seen overloading as one way to handle functions with optional data (like `compute_spread` with and without commission). Tomorrow you will learn a more compact alternative: default argument values, where a single function can be called with fewer arguments than it declares.

## Links

- [Roadmap](../cpp-roadmap.md)
- Previous: [Day 7 — Functions: Pass by Value, Ref, Const Ref](2026-05-06-day-07-functions-pass-by-value-ref-const-ref.md)
