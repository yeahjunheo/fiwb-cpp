---
date: 2026-05-12
day: 8
phase: 01-foundations
tags: [cpp, quant, functions, overloading]
---

# Day 8 — Function Overloading

> **Why this matters:** Yesterday you learned how to write a function and choose how its arguments are passed. Today you learn that C++ lets you give multiple functions the *same name* as long as they accept different parameters. This is called **function overloading**. It lets you write one logical concept — "print a value", "validate a field" — once per type, under a single memorable name, and have the compiler automatically pick the right version for each call. Understanding overloading also gives you your first look at how the compiler *reasons about types* when it reads a function call.

## Concept

### The problem overloading solves

Suppose you want to print a diagnostic line for both a trade quantity (an integer number of shares) and a price (a decimal number). Without overloading you would need two distinct names:

```cpp
void print_quantity(int qty)      { /* ... */ }
void print_price(double price)    { /* ... */ }
```

This works, but forces callers to remember two separate names for what is logically the same action. C++ lets you instead write both under a single name `print_value`, and the compiler picks the right one automatically:

```cpp
void print_value(int qty)         { /* ... */ }
void print_value(double price)    { /* ... */ }
```

### What makes two functions valid overloads

Two functions with the same name are valid overloads when they differ in their **parameter list** — the types, number, or order of parameters. All three of the following pairs are legal overloads:

```cpp
// Different number of parameters
void log(int code)              { /* ... */ }
void log(int code, double qty)  { /* ... */ }

// Different parameter types
void describe(int x)            { /* ... */ }
void describe(double x)         { /* ... */ }

// Different order of parameters
void record(int qty, double price)   { /* ... */ }
void record(double price, int qty)   { /* ... */ }
```

Notice that all three criteria involve the *parameter list* — not the return type.

### What does NOT create a valid overload

**Return type alone is not enough.** If you write:

```cpp
int    compute();    // version A
double compute();    // ERROR — not an overload, this is a redefinition
```

The compiler rejects this. The reason: at a call site like `compute();` (result discarded), or even `int x = compute();`, there is not always enough information to know which version you want. The return type is invisible to overload selection.

**Top-level `const` on a value parameter does not create a distinct overload either.** The `const` in `void f(const int x)` only constrains the *body* of `f` — it is invisible to the caller. So `void f(int x)` and `void f(const int x)` are the same function and the compiler will tell you so with a "redefinition" error.

However, `const` *does* matter when it is part of a reference:

```cpp
void set(int& x)          { /* modifies x */ }    // valid overload
void set(const int& x)    { /* read-only */  }    // different overload — const& accepts temporaries
```

These are distinct because `const int&` and `int&` bind differently, and callers get meaningfully different behavior.

### How the compiler picks the right overload — overload resolution

When you call an overloaded function, the compiler performs **overload resolution**: it looks at all functions with that name and ranks how closely each one matches the argument types you supplied. The ranking has three tiers, from best to worst:

1. **Exact match** — the argument type matches the parameter type directly (or with only a trivial adjustment, like the compiler adding `const`). This always wins over the lower tiers.

2. **Numeric promotion** — a small type is widened to a larger type with no loss of value. The standard promotions are `bool` → `int`, `char` → `int`, `short` → `int`, and `float` → `double`. Promotion ranks higher than a general conversion.

3. **Numeric conversion** — any other implicit type conversion: `int` → `double`, `double` → `int`, `int` → `long`, and so on.

The compiler selects the single overload with the *best-ranked* match for the argument. If two overloads tie at the same tier, the call is **ambiguous** — a compile error.

> **Key idea:** Overload resolution uses these tiers to find the most type-appropriate match. An exact match always wins. If no exact match exists, promotion wins over conversion. If two candidates are equally ranked, the compiler stops and tells you — it never silently picks one arbitrarily.

### A walkthrough of the tiers

Take `void f(int)` and `void f(double)`:

| Call | Argument type | Match for `f(int)` | Match for `f(double)` | Winner |
|------|--------------|--------------------|-----------------------|--------|
| `f(42)` | `int` | Exact | Conversion (`int`→`double`) | `f(int)` |
| `f(3.14)` | `double` | Conversion (`double`→`int`) | Exact | `f(double)` |
| `f(3.14f)` | `float` | Conversion (`float`→`int`) | Promotion (`float`→`double`) | `f(double)` |
| `f('A')` | `char` | Promotion (`char`→`int`) | Conversion (`char`→`double`) | `f(int)` |

Now consider `void f(int)` and `void f(long)` called as `f(3.14)`:

- `double` → `int` is a numeric conversion (tier 3).
- `double` → `long` is also a numeric conversion (tier 3).
- Both tie at tier 3 → **compile error: ambiguous call**.

The fix is to make the type explicit. You can write `f(static_cast<int>(3.14))` to force the `int` overload. (A **cast** — here `static_cast<T>(expr)` — explicitly converts a value to type `T`; we will cover casts fully in a later lesson.)

## Code Example

Create `20260512/day08.cpp`:

```cpp
// day08.cpp — function overloading in action

#include <iostream>
#include <cstdint>

// --- Overloads for describe ---
// The compiler picks the one whose parameter type matches the argument.

void describe(int32_t value) {
    std::cout << "Integer field: " << value << "\n";
}

void describe(double value) {
    std::cout << "Decimal field: " << value << "\n";
}

// --- Overloads for clamp ---
// clamp(x, lo, hi) returns lo if x < lo, hi if x > hi, otherwise x.
// Useful for keeping prices or quantities inside a valid range.

int32_t clamp(int32_t value, int32_t lo, int32_t hi) {
    if (value < lo) return lo;
    if (value > hi) return hi;
    return value;
}

double clamp(double value, double lo, double hi) {
    if (value < lo) return lo;
    if (value > hi) return hi;
    return value;
}

int main() {
    // describe: compiler picks based on argument type
    describe(100);          // int32_t literal  -> describe(int32_t)
    describe(99.75);        // double literal   -> describe(double)
    describe('A');          // char promotes to int -> describe(int32_t)

    std::cout << "\n";

    // clamp: keeps a quantity between 1 and 1000 shares
    int32_t qty = 1500;
    int32_t safe_qty = clamp(qty, 1, 1000);
    std::cout << "Raw quantity: " << qty << "  Clamped: " << safe_qty << "\n";

    // clamp: keeps a price between 0.01 and 99999.99
    double price = -5.0;
    double safe_price = clamp(price, 0.01, 99999.99);
    std::cout << "Raw price:    " << price << "  Clamped: " << safe_price << "\n";

    // --- Ambiguous call (kept as a comment — do NOT uncomment) ---
    // void f(int32_t) and void f(double) are defined above.
    // f(3.14f) would be unambiguous (float->double is a promotion, wins).
    // But: if you defined void g(int32_t) and void g(int64_t) and called g(3.14),
    // both conversions would tie and the compiler would error.

    return 0;
}
```

Compile and run:

```bash
mkdir -p 20260512
g++ -std=c++17 -Wall -Wextra 20260512/day08.cpp -o 20260512/day08
./20260512/day08
```

Expected output:

```
Integer field: 100
Decimal field: 99.75
Integer field: 65

Raw quantity: 1500  Clamped: 1000
Raw price:    -5  Clamped: 0.01
```

Notice that `describe('A')` prints `65` — the ASCII value of `'A'` — because `char` promotes to `int32_t` and the integer overload is selected. This is the promotion tier in action. Notice also that both `clamp` functions share one readable name even though their internal types differ completely.

## Quant Application

A trading system receives a continuous stream of **market data** — the live feed of prices and quantities from an exchange. (Market data = the stream of price and size updates that an exchange broadcasts to participants.) Each update typically carries several fields of different types: prices are stored as decimal numbers (`double` or a fixed-point type), quantities are integers, and identifiers might be short integers or character codes. A parser that converts raw bytes into these fields needs to handle all of them. With overloading, you can write a single `parse_field` function that accepts an `int32_t&` output for integer fields and a `double&` output for decimal fields. Every call site reads identically — `parse_field(buf, offset, value)` — and the compiler silently dispatches to the right version. Without overloading you would need `parse_int_field` and `parse_double_field` everywhere, doubling the names a maintainer must track.

The same pattern appears in **validation**. Before an order (an instruction to buy or sell a security at a given price and quantity) is sent to an exchange, a trading system checks that all its fields are within legal bounds — the price is positive and below some maximum tick, the quantity is between 1 and the exchange's maximum lot size. A `validate` function overloaded for `int32_t` (checking integer bounds) and `double` (checking decimal bounds with floating-point comparison) keeps the validation API uniform: every field goes through `validate(field, lo, hi)` regardless of type. The logic is written once per type, the interface stays clean.

In **HFT (high-frequency trading)** — trading that operates in single-digit microseconds, where each microsecond of latency has direct monetary cost — clean, overloaded interfaces matter for a subtler reason: they make the **hot path** (the code that runs once per incoming price update) easier to read and audit quickly. When a senior engineer reviews latency-critical code under time pressure, a single consistent name like `encode` or `normalize` that clearly dispatches by type is far safer than a proliferation of type-mangled names. Overloading is therefore both a programmer-ergonomics feature and a code-quality signal in production quant systems.

## Pitfalls

- **Trying to overload on return type only.** `int compute()` and `double compute()` with the same parameter list is a hard compiler error ("redefinition of 'compute'"), not an overload. The return type is never part of overload selection. If you need both, give them different names or add a parameter that distinguishes them.

- **Unexpected promotion selects the "wrong" overload.** Calling `describe(42)` when you have `describe(int32_t)` and `describe(double)` will always hit the integer version — but if you only wrote `describe(double)` and forgot `describe(int32_t)`, then `describe(42)` silently converts `42` to `double` and calls the double version. No warning, no error. Always double-check which overload a call resolves to when mixing integer and floating-point arguments.

- **`char` arguments surprising you.** `char` promotes to `int`, so `describe('A')` calls `describe(int32_t)`, not some `describe(char)` overload. If this surprises you (you expected the `double` or some other version), add a `describe(char)` overload explicitly or cast the argument.

- **Ambiguous calls stop compilation.** When two overloads tie at the same conversion tier, the compiler emits "call to overloaded function is ambiguous" and stops. The fix is always to make the type explicit at the call site — either pass the right literal type (`1` is `int`, `1.0` is `double`, `1L` is `long`) or use a cast.

- **Top-level `const` on a value parameter does not create a new overload.** Writing `void f(int x)` and `void f(const int x)` as two separate definitions produces a redefinition error. The `const` on a value parameter is only visible inside the function body and is stripped from the type for the purpose of overloading.

- **Defining too many overloads and losing track.** Overloading three or four versions of the same function is fine. But if you find yourself writing ten overloads, it is usually a sign that a different design — such as a function that accepts a more general type — would be cleaner. Keep overloads limited to genuinely distinct types that need different behavior.

## Exercise

Goal: write and observe function overloading with exact-match and promotion resolution.

1. Create `20260512/day08_ex.cpp`. Add `#include <iostream>` and `#include <cstdint>`.

2. Write two overloaded functions named `scale`:
   - `int32_t scale(int32_t value, int32_t factor)` — returns `value * factor` and prints `"integer scale"` before returning.
   - `double scale(double value, double factor)` — returns `value * factor` and prints `"double scale"` before returning.

3. In `main`, make the following four calls and print each result:
   ```cpp
   std::cout << scale(10, 3) << "\n";         // both args are int
   std::cout << scale(2.5, 4.0) << "\n";      // both args are double
   std::cout << scale(10, 3) << "\n";          // same as first — confirm it repeats
   ```
   **Before running**, write down which overload you predict each call will use.

4. Add a fifth call:
   ```cpp
   std::cout << scale(100, 2.0) << "\n";      // one int, one double — what happens?
   ```
   Try to compile. Read the error carefully. It should say something like "no matching function" or "ambiguous." Then fix it by explicitly writing both arguments as `double`: `scale(100.0, 2.0)`. Recompile and confirm it now prints `"double scale"` and `200`.

5. Compile and run the corrected program:
   ```bash
   g++ -std=c++17 -Wall -Wextra 20260512/day08_ex.cpp -o 20260512/day08_ex
   ./20260512/day08_ex
   ```
   Self-check: the first three calls should each print `"integer scale"` then the product; `scale(100.0, 2.0)` should print `"double scale"` then `200`. If you see an unexpected overload being called, add a print inside each function to confirm which one ran.

## Tomorrow's Preview

**Day 9 — Default Arguments.** You have seen that overloads let you use the same name for different parameter types. Tomorrow you will learn that C++ also lets you assign *default values* to parameters, so callers can omit trailing arguments that rarely change — a common pattern for functions with optional configuration.

## Links

- [Roadmap](../cpp-roadmap.md)
- Previous: [Day 7 — Functions: Pass by Value, Ref, Const Ref](2026-05-06-day-07-functions-pass-by-value-ref-const-ref.md)
