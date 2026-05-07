---
date: 2026-05-07
day: 8
phase: 01-foundations
tags: [cpp, quant, functions, overloading]
---

# Day 8 — Function Overloading

> **Why this matters:** Yesterday you wrote functions with specific, fixed parameter types. But what if you want a `compute_value` function that works on both an integer quantity and a floating-point price? Without overloading, you'd need two separately named functions like `compute_value_int` and `compute_value_double`. C++ lets you give both functions the same name — the compiler figures out which one to call based on the types of arguments you pass. This is called **function overloading**, and it is one of the most frequently used features in real C++ codebases.

## Concept

### The problem overloading solves

Imagine you are building a trading system and you want a function named `print_price`. Sometimes you have a price stored as an integer number of cents (e.g., `9975` meaning $99.75). Other times you have it as a `double` in dollars (e.g., `99.75`). Without overloading, you would need to pick two different names:

```cpp
void print_price_cents(int32_t cents);
void print_price_dollars(double dollars);
```

This works, but it puts a burden on the caller: they must remember which name to use for which type. If your system later adds a third numeric type, you add a third name, and so on. The function name starts to carry type-encoding that the compiler could handle for you.

**Function overloading** lets you define multiple functions with the same name in the same scope, as long as their parameter lists differ:

```cpp
void print_price(int32_t cents);          // version 1
void print_price(double  dollars);        // version 2
void print_price(int32_t qty, double px); // version 3 — different count
```

When you write `print_price(9975)`, the compiler sees that `9975` is an integer and picks version 1. When you write `print_price(99.75)`, it picks version 2. When you write `print_price(10, 99.75)`, it picks version 3. The selection happens entirely at **compile time** — there is no runtime overhead.

> **Key idea:** Two functions overload each other when they share a name but have different parameter types, different numbers of parameters, or both. The compiler picks the right version when you call it, based solely on the arguments you pass.

### What makes overloads distinct — and what does not

The **parameter list** is the only thing the compiler uses to tell overloads apart:

- Different number of parameters: always creates a distinct overload.
- Different types at any parameter position: creates a distinct overload.
- `const`-ness of a by-value parameter does **not** distinguish overloads. `void f(int x)` and `void f(const int x)` are not two overloads — they are a redefinition of the same function. (The `const` here only means the function itself cannot modify its local copy; it says nothing to the caller about the type being passed in.)

**The return type does not count.** You cannot have two overloads that differ only in return type:

```cpp
int    get_quantity();  // OK on its own
double get_quantity();  // ERROR: cannot overload on return type alone
```

This rule exists because at the call site the compiler often cannot infer which return type you want just from how the result is used. There would be no unambiguous way to choose. The compiler will reject this with a "redefinition" or "conflicting return type" error.

### How the compiler picks an overload (overload resolution)

When the compiler sees a function call, it goes through the following priority ranking to find the best match. It stops at the first step that produces exactly one winner.

**Step 1 — Exact match.** If one overload's parameter type matches the argument's type exactly, that overload wins immediately. This is the highest-priority outcome.

```cpp
void show(int32_t x);
void show(double  x);

show(42);    // 42 is an int literal  → exact match → show(int32_t)
show(3.14);  // 3.14 is a double literal → exact match → show(double)
```

**Step 2 — Numeric promotion.** If no exact match exists, the compiler considers **numeric promotions** — automatic widening of small integer types to larger ones. Specifically:
- `char`, `short`, and `bool` are promoted to `int`.
- `float` is promoted to `double`.

Promotions are only safe widenings; they never lose information. The compiler prefers them over general conversions.

```cpp
void show(int32_t x);

char c = 'A';
show(c);     // char → int promotion → calls show(int32_t)
```

**Step 3 — Standard conversion.** If neither exact match nor promotion works, the compiler tries broader conversions: `int` to `double`, `double` to `int` (with possible truncation), `int32_t` to `int64_t`, etc.

```cpp
void show(double x);

show(5);     // 5 is int; no exact-match double overload for int;
             // int → double is a standard conversion → calls show(double)
```

This three-step priority means the compiler always prefers the most specific match. An exact-match overload will always beat a promotion, which will always beat a conversion.

### Ambiguous calls — a compile error, not a surprise

If two overloads are equally good matches at the same step, the compiler cannot pick one and issues an **ambiguous call** error. This is caught at compile time, not at runtime.

The most common case is passing a `float` argument when you have `int32_t` and `double` overloads but no `float` overload:

```cpp
void show(int32_t x);
void show(double  x);

show(3.14f);  // 3.14f is a float literal
              // float → int32_t is a standard conversion
              // float → double  is a standard conversion
              // Both are equally ranked at Step 3 → AMBIGUOUS, compile error
```

The error message will say something like "call to 'show' is ambiguous." The fix is to add the missing overload:

```cpp
void show(float x);  // now 3.14f matches exactly
```

Or explicitly tell the compiler which type you mean by casting: `show((double)3.14f)`. The syntax `(TargetType)value` is a **C-style cast** — a way to force a value to be treated as a specific type. We will cover the preferred C++ casting syntax in a later phase; for now the C-style form works.

## Code Example

Create `20260507/day08.cpp`:

```cpp
// day08.cpp — function overloading: same name, different types

#include <iostream>
#include <cstdint>

// ------------------------------------------------------------------
// Three overloads of print_price
// ------------------------------------------------------------------

// Version 1: price as integer cents (e.g. 9975 = $99.75)
void print_price(int32_t cents) {
    std::cout << "Price (integer cents): " << cents << "\n";
}

// Version 2: price as a decimal dollar amount
void print_price(double dollars) {
    std::cout << "Price (double dollars): " << dollars << "\n";
}

// Version 3: quantity + price per unit → compute total value
void print_price(int32_t quantity, double price_per_unit) {
    double total = quantity * price_per_unit;
    std::cout << quantity << " units @ " << price_per_unit
              << " = " << total << "\n";
}

// ------------------------------------------------------------------
// Two overloads of absolute_value — one for integers, one for doubles
// ------------------------------------------------------------------

int32_t absolute_value(int32_t x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

double absolute_value(double x) {
    if (x < 0.0) {
        return -x;
    }
    return x;
}

int main() {
    // Compiler picks the right print_price overload based on argument type
    print_price(9975);        // int32_t argument → Version 1
    print_price(99.75);       // double argument  → Version 2
    print_price(10, 99.75);   // int32_t + double → Version 3

    std::cout << "\n";

    // absolute_value with the same name for both types
    int32_t a = absolute_value(-42);
    double  b = absolute_value(-3.14);
    std::cout << "absolute_value(-42)   = " << a << "\n";
    std::cout << "absolute_value(-3.14) = " << b << "\n";

    std::cout << "\n";

    // char argument — promoted to int, matches int32_t overload of print_price
    // (char is promoted to int in Step 2 of overload resolution)
    char code = 65;   // ASCII 65
    print_price(code);   // char → int32_t promotion

    return 0;
}
```

Compile and run (from inside the `20260507/` folder):

```bash
g++ -std=c++17 -Wall -Wextra day08.cpp -o day08
./day08
```

Expected output:

```
Price (integer cents): 9975
Price (double dollars): 99.75
10 units @ 99.75 = 997.5

absolute_value(-42)   = 42
absolute_value(-3.14) = 3.14

Price (integer cents): 65
```

Notice that the last line calls `print_price(code)` where `code` is a `char`. The compiler has no `char` overload, so it promotes `char` to `int32_t` (Step 2) and calls Version 1. The value `65` — the ASCII code for the letter 'A' — is printed as an integer, not as a character. This is the expected behavior of integer promotion.

Also notice that calling `absolute_value(-42)` returns an `int32_t`, while `absolute_value(-3.14)` returns a `double`. The same name, two completely separate functions in the compiled binary, picked by the compiler based on argument type.

## Quant Application

In a trading system, the same logical operation often needs to work on several different types of data. Consider a function named `log_event` that records what just happened in the system. Events might carry an integer sequence number, a floating-point price, or a message describing an error. Without overloading you need three names: `log_event_int`, `log_event_double`, `log_event_string`. With overloading you write one name and the compiler routes each call to the right version. This keeps the calling code uniform and readable: every place in the system that records an event just writes `log_event(...)`, regardless of what data it is passing.

The **order book** (a live record of all resting buy orders and sell orders for a security, organized by price level) is updated by many different kinds of events: a new order arriving with an integer quantity, a **fill** (a completed trade) carrying a floating-point executed price, a cancellation identified by an integer order ID. A function like `process_event` overloaded for each event type means the calling code all looks the same — `process_event(new_order)`, `process_event(fill)`, `process_event(cancel)` — even though the internal logic for each is different. This uniformity reduces the chance of calling the wrong variant by accident, because there is only one name to remember.

An important property of C++ function overloading: it has **zero runtime cost** compared to separately named functions. The compiler resolves which overload to call entirely at compile time — there is no runtime lookup, no branching, no table of function pointers. The generated machine code for `absolute_value(-42)` and `absolute_value_int(-42)` is identical. In HFT (high-frequency trading — systems that execute in single-digit microseconds where every nanosecond counts), this matters: developer-convenience features that the compiler completely eliminates are freely usable even on the critical path where speed is everything.

## Pitfalls

- **Thinking return type distinguishes overloads.** Writing `int f()` and `double f()` in the same scope is a compile error, not two overloads. Only parameter lists count. This is one of the most common misconceptions about overloading.

- **Ambiguous `float` calls.** Having `void f(int32_t)` and `void f(double)` but calling `f(3.14f)` (a `float` literal) produces a compile error: both conversions from `float` are equally ranked, so neither wins. The fix is to add `void f(float)`, or cast the argument to the type you intend: `f((double)3.14f)`.

- **`char` silently promoting to `int`.** If you have a `void f(int32_t)` overload and call `f(some_char_variable)`, you get the `int32_t` version because `char` promotes to `int`. This is correct behavior, but it can surprise you if you expected a special `char` version to exist and forgot to write it.

- **Defining an overload after it's needed.** All overloads that should be candidates for a call must be declared before that call in the source file. If you define `void show(double)` after the call to `show(3.14)`, the compiler will not see it as a candidate and will either pick a wrong overload or error. Always declare overloads before their call sites (or use header files, covered on Day 10).

- **Overloading when one function with a default argument would be clearer.** Two overloads that differ only in having an extra optional parameter can often be replaced by one function with a **default argument** (Day 9). Overloading adds a full second function definition; a default argument keeps it to one. Use overloading when the logic genuinely differs between types; use default arguments when one parameter is simply optional.

- **Top-level `const` does not create a new overload.** `void f(int x)` and `void f(const int x)` are not two overloads — they are the same function declared twice, and the compiler will reject the second as a redefinition. The `const` here only affects what the function can do with its local copy; it does not change the type of the argument the caller passes.

## Exercise

Goal: write a small overload set and observe the compiler choosing the right version for each call.

1. Create `20260507/day08_ex.cpp`. Add `#include <iostream>` and `#include <cstdint>`.

2. Write three overloads of a function named `record_trade`:
   - `void record_trade(int32_t quantity)` — prints `"Trade: quantity = "` followed by the quantity.
   - `void record_trade(double price)` — prints `"Trade: price = "` followed by the price.
   - `void record_trade(int32_t quantity, double price)` — prints quantity, price, and their product labelled as `"value = "`.

3. In `main`, write these calls:
   ```cpp
   record_trade(100);
   record_trade(49.99);
   record_trade(200, 52.50);
   ```
   **Before compiling**: write down on paper which overload each call will use and what each line of output will be.

4. Add a `char` variable `ch = 50;` and call `record_trade(ch)`. Predict which overload fires and what value is printed. Then confirm by compiling.

5. Compile and run:
   ```bash
   g++ -std=c++17 -Wall -Wextra day08_ex.cpp -o day08_ex
   ./day08_ex
   ```
   Self-check: you should see four lines of output. The `record_trade(ch)` call should invoke the `int32_t` overload (char promotes to int) and print `50`. If any of your predictions were wrong, re-read the overload resolution section and trace through the three steps for that call.

## Tomorrow's Preview

**Day 9 — Default Arguments.** You have seen that overloading lets you define multiple versions of a function for different parameter types. Tomorrow you will learn how to give a parameter a fallback value so callers can omit it — a lighter alternative to overloading when the logic is the same but one argument is often predictable.

## Links

- [Roadmap](../cpp-roadmap.md)
- Previous: [Day 7 — Functions: Pass by Value, Ref, Const Ref](2026-05-06-day-07-functions-pass-by-value-ref-const-ref.md)
