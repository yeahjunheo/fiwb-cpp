---
date: 2026-05-09
day: 8
phase: 01-foundations
tags: [cpp, quant, functions, overloading]
---

# Day 8 — Function Overloading

> **Why this matters:** Yesterday you learned that functions accept arguments of specific types — pass the wrong type, get a compile error. Today you discover that C++ lets you define multiple functions that share the same name, as long as their parameter lists differ. The compiler reads the arguments at the call site and picks the right version automatically. This is one of the features that makes C++ code read naturally: instead of `print_int(42)` and `print_double(3.14)`, you just write `print(42)` and `print(3.14)`.

## Concept

### The problem overloading solves

Imagine you're writing a small utility to display values. Without overloading, you'd need a different name for each type:

```cpp
void display_int(int32_t n)   { /* ... */ }
void display_double(double d) { /* ... */ }
```

The calling code is noisy — the programmer must always pick the right name. With overloading, both functions can be called `display`, and the compiler silently picks the right one based on the argument type. The calling code simply writes `display(value)`.

### What makes two functions valid overloads

Two functions are **overloads** of each other when they have the same name but different **parameter lists**. "Different" means at least one of these:

- **Different number of parameters.** `average(double a, double b)` and `average(double a, double b, double c)` are valid overloads.
- **Different parameter types.** `display(int32_t n)` and `display(double d)` are valid overloads.
- **Different order of types.** `process(int32_t id, double price)` and `process(double price, int32_t id)` are valid overloads (though this particular style is easy to misuse — callers must remember the order).

The functions live side by side and the compiler treats them as separate definitions.

### What does NOT create an overload: return type

Return type alone is **not** enough to differentiate two functions. The compiler cannot look at a call site like `average(10.0, 20.0)` and know which version you want based on what you plan to do with the result — you might not do anything with it at all. So this is a compile error:

```cpp
int    average(double a, double b) { return (int)(a + b) / 2; }
double average(double a, double b) { return (a + b) / 2.0; }
// ERROR: these have identical parameter lists
```

The rule is simple: the **return type is invisible to overload resolution**. Two functions with the same name and the same parameter list are always a conflict, regardless of their return types.

### How the compiler picks the right version: overload resolution

When you write `display(42)`, the compiler goes through a process called **overload resolution**:

1. **Collect candidates.** It gathers every function named `display` that is visible at that point.
2. **Find viable functions.** It keeps only those whose parameters can accept the arguments — either by exact match or via an implicit conversion (a rule-governed automatic type change the language performs silently).
3. **Rank by match quality.** Among the viable functions, it picks the one that requires the least conversion for each argument. An exact match always outranks a conversion.
4. **Select the best.** If one candidate is the best for every argument, that's the one called. If two candidates tie, the call is **ambiguous** and is a compile error.

> **Key idea:** The compiler always prefers an exact match over any implicit conversion. If there is one candidate that matches the argument type exactly, that candidate wins.

### A concrete ranking example

Suppose you have:

```cpp
void show(int32_t n);
void show(double d);
```

- `show(5)` — the literal `5` has type `int`, which is the same underlying type as `int32_t` on most platforms. Exact match for `show(int32_t)`. That version is called.
- `show(5.0)` — the literal `5.0` has type `double`. Exact match for `show(double)`. That version is called.
- `show(5.0f)` — `5.0f` is type `float`. There is no `show(float)`, so the compiler looks for a conversion. Converting `float` to `double` is a better match than converting `float` to `int32_t` (because `float`-to-`double` preserves all information; `float`-to-`int32_t` truncates). The `show(double)` version is called.

### Ambiguous calls: when the compiler cannot choose

If two candidates are equally good, the compiler refuses to compile and reports an ambiguous call error. A classic case: two overloads whose parameters require the same *amount* of conversion for the given arguments.

```cpp
void process(int32_t a, double b);
void process(double a, int32_t b);
```

Calling `process(1, 2)` — both arguments are plain `int`. For the first overload: `1` matches `int32_t` (exact), `2` needs to convert to `double`. For the second: `1` needs to convert to `double`, `2` matches `int32_t` (exact). Neither is uniformly better. The compiler reports an error. The fix is to make at least one argument unambiguous:

```cpp
process(1, 2.0);   // second arg is exact double → first overload wins
process(1.0, 2);   // first arg is exact double → second overload wins
```

### One nuance: top-level `const` on value parameters is ignored

When the compiler decides whether two declarations are different overloads, it ignores `const` applied directly to a **value** parameter. This is because `const` on a by-value parameter only affects the local copy inside the function — it has no effect on the caller, so it cannot be used to distinguish overloads from the caller's perspective.

```cpp
void f(int32_t n);         // declaration 1
void f(const int32_t n);   // declaration 2 — NOT a new overload; same as 1
```

These two declarations describe the same function. The `const` says "don't modify the local copy of `n` inside the function," which is a private implementation detail.

This is different from `const` on a reference or pointer, where `const` affects whether the function can modify the original variable — that *does* create a meaningful difference visible to callers. But that interaction comes up naturally when you start using classes (Day 22 onward), so for now the rule to remember is: **value-parameter `const` is invisible to overload resolution**.

## Code Example

Create the file `20260509/day08.cpp`:

```cpp
// day08.cpp — function overloading: same name, different parameter lists

#include <iostream>
#include <cstdint>

// --- Overloads by parameter TYPE ---

void print_value(int32_t n) {
    std::cout << "int32_t value: " << n << "\n";
}

void print_value(double d) {
    std::cout << "double  value: " << d << "\n";
}

// --- Overloads by parameter COUNT ---

double average(double a, double b) {
    return (a + b) / 2.0;
}

double average(double a, double b, double c) {
    return (a + b + c) / 3.0;
}

// This would be a compile error — return type alone does NOT create an overload:
//   int average(double a, double b) { ... }   // ERROR: same param list as above

int main() {
    // --- Type-based overload resolution ---
    print_value(42);       // 42 is an integer  → picks print_value(int32_t)
    print_value(3.14);     // 3.14 is a double  → picks print_value(double)
    print_value(7.0f);     // 7.0f is a float; float->double is a better conversion
                           // than float->int32_t → picks print_value(double)

    std::cout << "\n";

    // --- Count-based overload resolution ---
    double avg2 = average(10.0, 20.0);
    double avg3 = average(10.0, 20.0, 30.0);

    std::cout << "average(10, 20)     = " << avg2 << "\n";
    std::cout << "average(10, 20, 30) = " << avg3 << "\n";

    return 0;
}
```

Compile and run:

```bash
mkdir -p 20260509
g++ -std=c++17 -Wall -Wextra 20260509/day08.cpp -o 20260509/day08
./20260509/day08
```

Expected output:

```
int32_t value: 42
double  value: 3.14
double  value: 7

average(10, 20)     = 15
average(10, 20, 30) = 20
```

Notice that `print_value(7.0f)` printed `7` via the `double` overload — the `float` argument was silently promoted to `double` because that conversion is a closer fit than `float`-to-`int32_t`. The `double` overload won. The `average` functions both exist and the compiler selects by argument count.

## Quant Application

Exchanges represent prices in two common formats: **floating-point** (e.g., `99.75` as a `double`) and **fixed-point** (e.g., `9975` as an `int32_t`, meaning 9975 hundredths of a dollar). A tick — the minimum allowed price increment — is often one hundredth or one thousandth of a dollar depending on the exchange. When you write utility functions for a trading system, you frequently want the same logical operation to work on both representations. Overloading lets you write `format_price(double)` and `format_price(int32_t)` under one name so callers never have to think about which version applies to their data type.

A practical example is a clamping function — one that constrains a value to lie within a valid range. In order management, you might want to clamp an order quantity to a per-account limit (an integer) and also clamp a price to a valid band around the current market (a `double`). Without overloading you'd write `clamp_int` and `clamp_double`. With overloading you write two functions both named `clamp`, and the type of the argument determines which one runs. The call site reads cleanly: `clamp(requested_qty, 1, 10000)` for integer quantities and `clamp(requested_price, lower_bound, upper_bound)` for floating-point prices.

In real HFT codebases, overloading appears most visibly in logging and serialization layers — infrastructure that runs on every single message. A logger might have `log_field(const char* label, int32_t value)` and `log_field(const char* label, double value)` so that any numeric field can be logged with a single consistent call. The developer writing a new message parser doesn't need to look up a type-specific log function; they just call `log_field`. This reduction in mental overhead matters in systems where developers must reason simultaneously about microsecond latency budgets and complex protocol state machines.

## Pitfalls

- **Trying to overload on return type only.** Writing two functions with the same name and the same parameter list but different return types is a compile error. The compiler cannot distinguish which one to call because the call site `average(a, b)` carries no information about the expected return type.

- **Ambiguous calls from implicit conversions.** If no candidate has an exact-match advantage over the other, the compiler reports an ambiguous call error and refuses to compile. This most often happens with mixed-type integer arguments spread across two overloads that each match one argument exactly and convert the other. Fix it by passing explicit types at the call site, or by adding a more specific overload.

- **Top-level `const` on value parameters does not create a new overload.** Writing both `void f(int32_t n)` and `void f(const int32_t n)` does not give you two overloads — it gives you one definition of `f`. The `const` is a private implementation detail, invisible to the caller.

- **Exact match wins silently over implicit conversion.** If you expect a call to route to the `double` overload but an `int32_t` overload also exists, passing an integer literal will always call the `int32_t` version. This can cause subtle bugs if the two versions have different behavior that you assumed would only trigger for floats.

- **Different parameter names are not different overloads.** `void process(int32_t price)` and `void process(int32_t quantity)` have identical parameter lists; the names `price` and `quantity` are documentation, not part of the function's signature. This is a redefinition of the same function, not an overload.

- **Overloads with too-similar parameter lists cause maintenance confusion.** `process(int32_t, double)` and `process(double, int32_t)` are technically valid overloads, but callers must remember the argument order carefully. Prefer overloads that are clearly distinct (different count, or clearly different types) and use descriptive function names when the distinction is subtle.

## Exercise

Goal: write your own overloaded functions, call them with different argument types, and observe that the compiler routes each call to the correct version.

1. Create `20260509/day08_ex.cpp`. Add `#include <iostream>` and `#include <cstdint>`.

2. Write two overloads of `describe`:
   - `void describe(int32_t n)` — prints `"Integer: "` followed by `n`.
   - `void describe(double d)` — prints `"Double:  "` followed by `d`.

3. Write two overloads of `add`:
   - `int32_t add(int32_t a, int32_t b)` — returns `a + b`.
   - `double  add(double a, double b)`   — returns `a + b`.

4. In `main`:
   - Call `describe(100)`, `describe(3.14)`, and `describe(2.5f)`. Before running, predict which overload each call picks and why.
   - Call `add(3, 4)` and print the result. Call `add(3.5, 1.5)` and print the result. Verify that integer addition returns an `int32_t` and the float call routes to the `double` version.

5. Compile and run:
   ```bash
   g++ -std=c++17 -Wall -Wextra 20260509/day08_ex.cpp -o 20260509/day08_ex
   ./20260509/day08_ex
   ```
   Self-check: `describe(100)` should say "Integer", `describe(3.14)` should say "Double", and `describe(2.5f)` should also say "Double" because `float` converts to `double` more cleanly than to `int32_t`. `add(3, 4)` should print `7` and `add(3.5, 1.5)` should print `5`.

## Tomorrow's Preview

**Day 9 — Default Arguments.** You can assign a default value to a function parameter so callers can omit it. Tomorrow you will see how defaults interact with overloading and the rules the compiler enforces to keep things unambiguous.

## Links

- [Roadmap](../cpp-roadmap.md)
- Previous: [Day 7 — Functions: Pass by Value, Ref, Const Ref](2026-05-06-day-07-functions-pass-by-value-ref-const-ref.md)
