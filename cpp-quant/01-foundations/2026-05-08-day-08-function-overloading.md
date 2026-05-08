---
date: 2026-05-08
day: 8
phase: 01-foundations
tags: [cpp, quant, functions, overloading]
---

# Day 8 — Function Overloading

> **Why this matters:** Yesterday you learned how to write functions and control how data flows in and out of them. Today you discover that C++ lets you give *different* functions the same name as long as their parameter lists differ — and the compiler automatically picks the right version based on what you pass in. This feature, called **function overloading**, is one of the first places where C++ does real work for you: instead of maintaining a separate name for every type combination, you write one meaningful name and let the compiler do the bookkeeping.

## Concept

### The problem overloading solves

Imagine you want a function called `log_value` that prints a number. Without overloading, you need a distinct name for each type:

```cpp
void log_int(int32_t n)   { std::cout << "int: "    << n << "\n"; }
void log_double(double n) { std::cout << "double: " << n << "\n"; }
```

Every call site must know which name matches which type. As the number of types grows, so does the cognitive load. With overloading, both functions live under the same name:

```cpp
void log_value(int32_t n) { std::cout << "int: "    << n << "\n"; }
void log_value(double n)  { std::cout << "double: " << n << "\n"; }
```

Now you write `log_value(42)` and `log_value(3.14)` — one name, two behaviors, zero confusion about which to use.

### The function signature: what makes overloads distinct

C++ identifies each function by its **signature** — the combination of its name and the types of its parameters, in order. Two functions with the same name but different parameter lists are *separate functions* as far as the compiler is concerned.

**What CAN create a distinct signature:**

- Different number of parameters: `void f(int32_t)` vs `void f(int32_t, int32_t)`
- Different parameter types: `void f(int32_t)` vs `void f(double)`
- Different ordering of types: `void f(int32_t, double)` vs `void f(double, int32_t)`

**What CANNOT create a distinct signature:**

- **Return type alone.** `int32_t f(int32_t)` and `double f(int32_t)` is not valid overloading. The compiler must choose the right function *before* it knows what the return value will be used for, so return type is excluded from the signature. Attempting this produces a compile error along the lines of "functions that differ only in their return type cannot be overloaded."
- **Parameter names.** `void f(int32_t price)` and `void f(int32_t quantity)` are the *same* function — just with different names for the parameter. Names are documentation; only the types go into the signature.

> **Key idea:** The compiler picks an overload based on the parameter types and count. Return type and parameter names play no role in the selection.

### Overload resolution: how the compiler picks

When you write `log_value(42)`, the compiler runs a process called **overload resolution** to select the best match. It works in three steps:

**Step 1 — Collect candidates.** The compiler finds all functions named `log_value` that are visible in the current scope.

**Step 2 — Determine viable candidates.** A candidate survives if the number of arguments you provided matches its parameter count *and* each argument can somehow be converted to the matching parameter type.

**Step 3 — Rank and pick the best.** The compiler scores each viable candidate using a priority ranking. Lower rank number means a better (cheaper) match:

| Rank | Category | Example |
|------|----------|---------|
| 1 — best | Exact match | Passing `int32_t` to an `int32_t` parameter |
| 2 | Promotion | `char` → `int32_t` (small integer widens to larger integer); `float` → `double` |
| 3 | Standard conversion | `int32_t` → `double` (integer converted to floating-point) |

The winner is the overload that beats or ties every other candidate on every argument, without losing on any. If two overloads are *equally good* — both require the same rank for a given argument — the compiler cannot pick a winner and reports a **compile error: ambiguous call to overloaded function**.

This is worth pausing on. The compiler *refusing* to guess is a feature, not a flaw. An ambiguous call would be a silent bug; catching it at compile time is far better than running the wrong function at runtime.

### Promotion: a close look at rank 2

**Promotion** is when a smaller numeric type is automatically widened to a larger type of the same kind. The most common promotions you will encounter:

- `char` → `int32_t` (a character's numeric code becomes a full integer)
- `short` → `int32_t`
- `float` → `double`

Promotions rank higher (closer to exact) than standard conversions because no information is lost and the hardware cost is minimal.

### Walking through examples

Given:

```cpp
void f(int32_t n) { }
void f(double n)  { }
```

| Call | Overload selected | Why |
|------|-------------------|-----|
| `f(42)` | `f(int32_t)` | Integer literal `42` has type `int`, same as `int32_t` on this platform — exact match |
| `f(3.14)` | `f(double)` | Double literal `3.14` — exact match |
| `f(3.14f)` | `f(double)` | `3.14f` is a `float`; `float`→`double` is promotion (rank 2), beats `float`→`int32_t` (rank 3) |
| `f('A')` | `f(int32_t)` | `char`→`int32_t` is promotion (rank 2), beats `char`→`double` (rank 3) |

Now a case that triggers an ambiguity error:

```cpp
long val = 100L;
f(val);  // COMPILE ERROR: ambiguous
         // long→int32_t is rank 3 (standard conversion)
         // long→double   is rank 3 (standard conversion)
         // both equally bad — compiler refuses to choose
```

The fix is to cast explicitly so the intent is unambiguous:

```cpp
f(static_cast<int32_t>(val));   // exact match with f(int32_t)
f(static_cast<double>(val));    // exact match with f(double)
```

`static_cast<T>(expr)` is a way to explicitly ask the compiler to convert `expr` to type `T`. We will cover casts in detail on a future day; for now, treat it as "I am intentionally converting this value to type T."

### Multiple parameters

Overload resolution works argument-by-argument when there are multiple parameters. The winning overload must win (or tie) on *every* argument:

```cpp
void g(int32_t a, double b)  { }
void g(double  a, int32_t b) { }

g(42, 3.14);   // calls g(int32_t, double)  — both args are exact matches
g(3.14, 42);   // calls g(double, int32_t)  — both args are exact matches
g(42, 42);     // AMBIGUOUS
               // g(int32_t, double): arg1 exact (rank 1), arg2 int→double (rank 3)
               // g(double, int32_t): arg1 int→double (rank 3), arg2 exact (rank 1)
               // first overload wins arg1 but loses arg2; second is the reverse
               // neither dominates — compile error
```

This is a case where having exactly the overloads you need — no more — avoids surprises.

## Code Example

Create `20260508/day08.cpp`:

```cpp
// day08.cpp — function overloading in action

#include <iostream>
#include <cstdint>

// Three overloads of print_price — same name, different parameter lists
void print_price(int32_t ticks) {
    std::cout << "int32_t:  " << ticks << " ticks\n";
}

void print_price(double dollars) {
    std::cout << "double:   $" << dollars << "\n";
}

void print_price(int32_t quantity, double price) {
    std::cout << "order:    " << quantity << " shares @ $" << price << "\n";
}

// Overloaded clamp: keep a value within [lo, hi]
int32_t clamp(int32_t val, int32_t lo, int32_t hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

double clamp(double val, double lo, double hi) {
    if (val < lo) return lo;
    if (val > hi) return hi;
    return val;
}

int main() {
    // Overload selected by argument type
    print_price(250);           // integer → print_price(int32_t)
    print_price(99.75);         // double  → print_price(double)
    print_price(100, 150.25);   // two args → print_price(int32_t, double)

    std::cout << "\n";

    // char promotes to int32_t — 'A' has ASCII value 65
    char letter = 'A';
    print_price(letter);        // char→int32_t is a promotion; calls print_price(int32_t)

    std::cout << "\n";

    // clamp selects the right overload by argument types
    int32_t qty = clamp(500, 1, 100);       // 500 clamped to 100
    double  pct = clamp(1.75, 0.0, 1.0);   // 1.75 clamped to 1.0

    std::cout << "clamped qty: " << qty << "\n";
    std::cout << "clamped pct: " << pct << "\n";

    return 0;
}
```

Compile and run:

```bash
mkdir -p 20260508
g++ -std=c++17 -Wall -Wextra 20260508/day08.cpp -o 20260508/day08
./20260508/day08
```

Expected output:

```
int32_t:  250 ticks
double:   $99.75
order:    100 shares @ $150.25

int32_t:  65 ticks

clamped qty: 100
clamped pct: 1
```

Notice that `'A'` printed as `65` — the compiler promoted `char` to `int32_t` and called the integer overload, which treated the value as a plain number (the ASCII code of `'A'`). This is expected behavior and illustrates that overload selection uses types, not values.

Also notice that `clamp(500, 1, 100)` selected `clamp(int32_t, int32_t, int32_t)` while `clamp(1.75, 0.0, 1.0)` selected `clamp(double, double, double)` — the compiler matched each call site to the appropriate implementation without any explicit indication from you.

## Quant Application

Quant code constantly works with two distinct numeric worlds: **integer quantities** (the number of shares or contracts in an order — you cannot buy half a share on most exchanges, so share counts are always integers) and **floating-point prices** (prices like `$99.7500` that require decimal precision). A trading system regularly needs utility functions — clamping a value to valid bounds, rounding, printing for logs — that must work correctly on both types. Without overloading, you need names like `clamp_int` and `clamp_double` scattered throughout tens of thousands of lines of code. With overloading you write one `clamp`, and the compiler routes each call to the right implementation. Code at the call site reads naturally ("clamp this quantity to the valid order size range") while the type-specific logic lives cleanly in separate definitions.

The C++ standard library itself is built on this principle. The math functions `std::abs`, `std::min`, `std::max`, and dozens of others are each provided for both integer and floating-point types — you call `std::abs(-5)` or `std::abs(-3.14)` without caring which underlying implementation runs. (The standard library achieves this using a feature called *templates* — for now, think of a template as a recipe that generates overloads automatically for any type you give it; we will cover templates on Day 64. For the moment, the effect at the call site is exactly like overloading: one function name, multiple type-specific implementations.) In a real trading system you encounter these functions every day when computing absolute price changes, clamping order sizes, or finding the minimum of two bid prices.

In an **order book** — the live list of all resting buy and sell orders organized by price for a given security — a price can be represented in two common ways: as an integer count of the smallest price increment (called a **tick**, e.g., one tick might equal `$0.0001` for certain equities), or as a floating-point dollar value. Different layers of a trading system use different representations: the network layer that parses raw exchange data works in ticks (fast, no floating-point rounding); the risk management layer that computes profit and loss works in dollars. Overloading utility functions such as `to_display_string` or `round_to_tick` for both integer-tick and double-dollar representations keeps both layers clean without forcing a type conversion at every boundary. As your system grows, one expressive function name per operation — with type-specific overloads behind the scenes — becomes a meaningful maintainability win.

## Pitfalls

- **Overloading on return type only.** Writing `int32_t get()` and `double get()` in the same scope produces a compile error. The fix is either different names or adding a parameter that distinguishes the overloads.

- **Ambiguous calls you did not expect.** Adding an overload for a new type can make previously valid calls ambiguous. If you had `void f(double)` and then add `void f(long)`, existing calls like `f(42)` may now fail because `int→double` and `int→long` are both standard conversions at equal rank. Always test existing call sites after adding a new overload.

- **`char` silently promoting to `int`.** If you have `void f(int32_t)` and `void f(double)`, a call `f('X')` will call `f(int32_t)` with the ASCII value of `'X'` — `88` — which is probably not what you intended. Add a `void f(char)` overload if you need character-specific behavior.

- **Parameter names are invisible to the signature.** Writing `void f(int32_t price)` and `void f(int32_t quantity)` in the same scope is a *redefinition* error — not two overloads. Only the types matter.

- **Assuming the compiler picks the "obvious" overload.** The ranking rules (exact match → promotion → standard conversion) are precise and can produce surprising results when types mix. When in doubt, cast the argument explicitly: `static_cast<int32_t>(x)` removes ambiguity and documents intent.

- **Confusing overloading with overriding.** *Overloading* is what this lesson covers: same name, different parameter types, in the same scope. *Overriding* applies to classes and virtual functions (Day 32). The words sound similar; they describe entirely different mechanisms.

## Exercise

Goal: write and observe three overloaded functions, trigger a promotion, and confirm that return-type-only overloading fails to compile.

1. Create `20260508/day08_ex.cpp` with `#include <iostream>` and `#include <cstdint>`. Write two overloads of `describe` before `main`:
   - `void describe(int32_t n)` — prints `"integer: "` followed by `n` and a newline.
   - `void describe(double n)` — prints `"double: "` followed by `n` and a newline.

2. In `main`, call:
   - `describe(10)` — predict which overload runs and why.
   - `describe(2.5)` — predict which overload runs and why.
   - `describe('Z')` — predict what happens (hint: `'Z'` is a `char`; its ASCII value is `90`). Run and verify the output matches your prediction.

3. Add a third overload `void describe(int32_t a, int32_t b)` that prints `"pair: "` followed by `a`, a space, and `b`. Call `describe(3, 7)` in `main` and verify it selects the two-parameter version.

4. **Intentional error step:** add the line `double describe(int32_t n) { return n * 1.0; }` just below your existing `void describe(int32_t n)`. Try to compile. Read and record the error message — it will tell you the functions differ only in return type. Then remove that line before the final compile.

5. Compile and run the final (error-free) program:
   ```bash
   g++ -std=c++17 -Wall -Wextra 20260508/day08_ex.cpp -o 20260508/day08_ex
   ./20260508/day08_ex
   ```
   Self-check: `describe(10)` → `integer: 10`; `describe(2.5)` → `double: 2.5`; `describe('Z')` → `integer: 90`; `describe(3, 7)` → `pair: 3 7`. The error in step 4 should mention "return type" or "cannot be overloaded."

## Glossary

- **Overload resolution** — the compiler process of selecting the best matching function among candidates with the same name; covered in full today, relied on implicitly every time you call a function for the rest of the curriculum.
- **Promotion** — automatic widening of a small numeric type to a larger type of the same kind (`char`→`int32_t`, `float`→`double`); part of the type system from Day 2, named and ranked today.
- **`static_cast<T>(expr)`** — an explicit type conversion that tells the compiler "treat `expr` as type `T`"; introduced here to resolve ambiguity; covered fully on a future day.

## Tomorrow's Preview

**Day 9 — Default Arguments.** Now that you know how the compiler picks among overloads, you will learn how to give parameters default values — so callers can omit trailing arguments, and the compiler fills them in automatically. Default arguments and overloading interact in subtle ways that Day 9 will address head-on.

## Links

- [Roadmap](../cpp-roadmap.md)
- Previous: [Day 7 — Functions: Pass by Value, Ref, Const Ref](2026-05-06-day-07-functions-pass-by-value-ref-const-ref.md)
