---
date: 2026-05-13
day: 8
phase: 01-foundations
tags: [cpp, quant, functions, overloading]
---

# Day 8 — Function Overloading

> **Why this matters:** Yesterday you learned how to write functions and choose how to pass arguments. Today you discover that C++ lets you give multiple functions the *same name*, as long as their parameter lists differ. The compiler automatically picks the right version based on what you pass in. This keeps code readable — one natural name for one logical operation — and introduces an important mental model: the compiler matches calls to functions by examining argument *types*, not just names.

## Concept

### The problem overloading solves

Imagine you want a function that prints a value for logging. You have two kinds of data: quantities (whole numbers of shares, stored as `int32_t`) and prices (fractional values, stored as `double`). Without overloading you need two different names:

```cpp
void print_int_value(int32_t x)  { ... }
void print_double_value(double x) { ... }
```

That works, but it forces you to remember two names for what is logically one operation: "print this value." C++ solves this with **function overloading**: you can give both functions the same name, and the compiler figures out which one to call.

```cpp
void print_value(int32_t x) { ... }
void print_value(double x)  { ... }
```

Both functions share the name `print_value`. Together they form an **overload set** — a collection of functions with the same name visible in the same scope.

### The rule: overloads must differ in their parameter lists

The compiler distinguishes overloads by **parameter types and count**. Two functions can share a name as long as at least one of the following differs:

- The **number** of parameters, or
- The **type** of at least one parameter.

Two things that do **not** create a distinct overload:

- **Return type alone.** You cannot have two functions named `f` that differ only in what they return. At the call site `f(x)`, the compiler sees only the argument types — it has no way to decide which return type you want. Writing two such functions is a **redefinition error** (a compile error), not an overload.

- **Top-level `const` on a by-value parameter.** `void f(int32_t x)` and `void f(const int32_t x)` look different, but to the compiler they are the same signature. The `const` there only affects what the function can do with its own local copy; the caller's side is identical. Writing both is also a redefinition error.

### How the compiler picks: overload resolution

When you write `print_value(42)`, the compiler runs a process called **overload resolution**:

1. **Build the candidate set.** Collect all functions named `print_value` visible at this point.
2. **Filter for viable functions.** Keep only those where the argument count matches and where each argument can be implicitly converted to the matching parameter type.
3. **Find the best viable function.** Rank the remaining candidates by how well the argument types match the parameter types, and pick the one that wins.

The ranking from best to worst:

| Match quality | Example |
|---|---|
| **Exact match** | Passing `int32_t` to an `int32_t` parameter |
| **Numeric promotion** | Passing `float` to a `double` parameter (widens without data loss) |
| **Standard conversion** | Passing `int32_t` to a `double` parameter (different type, but still safe) |

The compiler always prefers an exact match over a promotion, and a promotion over a standard conversion.

> **Key idea:** The compiler reads the types of the arguments you pass and picks the overload whose parameters require the fewest and least lossy conversions.

### Ambiguous calls

If two overloads are equally good matches for a given call, the compiler refuses to pick arbitrarily — the call is **ambiguous** and you get a compile error. This is intentional: the compiler forces you to be explicit rather than silently calling the wrong function.

A classic example:

```cpp
void g(int32_t a, double b)  { ... }
void g(double a, int32_t b)  { ... }

g(1, 2);  // compile error: ambiguous
```

For `g(1, 2)`:
- Matching `g(int32_t, double)`: first arg exact, second arg needs a standard conversion. One conversion.
- Matching `g(double, int32_t)`: first arg needs a standard conversion, second arg exact. Also one conversion.

Neither candidate is strictly better in all positions. The compiler reports an ambiguity error. The fix is to be explicit at the call site: `g(1, 2.0)` unambiguously calls the first overload, and `g(1.0, 2)` calls the second.

### Overloading with const references

You can also overload on whether a parameter is a mutable reference or a `const` reference:

```cpp
void tag(int32_t& x)       { std::cout << "mutable ref\n"; }
void tag(const int32_t& x) { std::cout << "const ref\n"; }
```

When you pass a plain non-`const` variable, the compiler prefers the mutable reference overload — it is a more precise match. When you pass a `const` variable or a temporary value (like a literal), only the `const` reference overload is viable, because a mutable reference cannot bind to those. This automatic routing enforces const-correctness: objects marked `const` are always directed to functions that promise not to modify them.

You will write these paired overloads mainly when designing your own types (Phase 2). The important thing for now is that `const`-ness participates in overload selection.

## Code Example

Create `20260513/day08.cpp`:

```cpp
// day08.cpp — function overloading

#include <iostream>
#include <cstdint>

// --- Overload set 1: "print_value" for int32_t or double ---
void print_value(int32_t x) {
    std::cout << "int32_t value: " << x << "\n";
}

void print_value(double x) {
    std::cout << "double  value: " << x << "\n";
}

// --- Overload set 2: "compute_fee" with or without an explicit rate ---
double compute_fee(double price) {
    return price * 0.001;          // flat 0.1% fee
}

double compute_fee(double price, double rate) {
    return price * rate;           // caller-supplied rate
}

// --- Overload set 3: mutable ref vs const ref ---
void tag(int32_t& x) {
    std::cout << "  mutable ref, can modify. x = " << x << "\n";
}

void tag(const int32_t& x) {
    std::cout << "  const ref, read-only.  x = " << x << "\n";
}

int main() {
    // Overload set 1
    print_value(100);        // exact match -> print_value(int32_t)
    print_value(99.75);      // exact match -> print_value(double)
    std::cout << "\n";

    // Overload set 2
    double fee1 = compute_fee(200.0);           // 1-parameter overload
    double fee2 = compute_fee(200.0, 0.005);    // 2-parameter overload
    std::cout << "flat fee:     " << fee1 << "\n";
    std::cout << "variable fee: " << fee2 << "\n";
    std::cout << "\n";

    // Overload set 3: routing depends on const-ness
    int32_t qty = 50;
    const int32_t max_qty = 1000;

    tag(qty);        // non-const variable -> mutable ref overload
    tag(max_qty);    // const variable    -> const ref overload (only viable one)
    tag(42);         // literal/temporary -> const ref overload (only viable one)

    return 0;
}
```

Compile and run (from inside `20260513/`):

```bash
g++ -std=c++17 -Wall -Wextra day08.cpp -o day08
./day08
```

Expected output:

```
int32_t value: 100
double  value: 99.75

flat fee:     0.2
variable fee: 1

  mutable ref, can modify. x = 50
  const ref, read-only.  x = 1000
  const ref, read-only.  x = 42
```

Notice that `qty` routes to the mutable-reference overload while `max_qty` and the literal `42` both route to the const-reference overload. The compiler enforces const-correctness automatically through overload selection — you never had to write an `if`-statement for this.

## Quant Application

In a trading system, the core data falls into two categories with very different types: **prices** (the value at which a security trades, stored as floating-point) and **quantities** (the number of shares or contracts, stored as integers). A **tick** is a single price or quote update broadcast by an exchange. You often need the same logical operation — validate a range, format for display, write to a log — applied to both kinds of data. Without overloading you would need separate names like `validate_price` and `validate_quantity`. As a system grows to dozens of functions, this naming sprawl becomes hard to read and maintain. Overloading keeps one clean name per concept while letting the compiler route calls correctly based on argument type.

A subtler benefit is defense against silent conversions. Suppose you write a single function `void log_field(double x)` and accidentally call it with an integer quantity. The integer silently converts to `double` — no warning, no error — and you may not notice until the number shows up in a log as `50.0` where a downstream parser expected `50`. By providing `void log_field(int32_t x)` and `void log_field(double x)` as overloads, each type routes to the correct version. Any *unexpected* type at the call site — say, a `float` where only `int32_t` or `double` are expected — may trigger an ambiguity error at compile time rather than silently misbehaving at runtime. Catching errors at compile time, before any code runs, is a core discipline in latency-sensitive systems where a runtime surprise can cost real money.

Understanding overload resolution also matters when reading the error messages that real trading codebases produce. When the compiler says "no matching function for call to `submit_order`" or "call is ambiguous," it is telling you something specific about the argument types at that call site. Engineers who understand overload resolution read those errors as diagnostic clues — "the argument type I passed doesn't match any overload, or it matches two equally well" — and fix the mismatch quickly. Engineers who don't understand it tend to comment out overloads at random until it compiles, which is dangerous in a production codebase.

## Pitfalls

- **Differing only in return type is a compile error, not an overload.** Writing `int32_t get() { ... }` and `double get() { ... }` in the same scope is illegal. The compiler cannot tell which you want from a call like `get()` — there are no argument types to inspect.

- **Unexpected standard conversion calling the wrong overload.** If you have `void f(double x)` and call `f(42)`, the integer `42` converts to `double` and `f(double)` is called. This is correct behavior, but surprises you if you expected an `int32_t` overload and forgot to write one. The fix is to add the missing overload.

- **Ambiguous calls with mixed integer and floating-point literals.** `g(1, 2)` where two overloads each require one conversion (as shown above) is a compile error. Fix it by making the types explicit at the call site: `g(1, 2.0)` or `g(int32_t(1), int32_t(2))`.

- **Confusing overloading with redefinition.** Two functions with the same name *and* the same parameter types in the same scope is a **redefinition error**. Overloading only works when the parameter lists are genuinely different.

- **Forgetting that top-level `const` on a value parameter does not count.** `void f(int32_t x)` and `void f(const int32_t x)` are the same overload; writing both is a redefinition error, not an overload.

- **Confusing overloading with overriding.** Overloading (same name, different parameters, same scope) is completely different from *overriding* (replacing a virtual function in a derived class, covered in Phase 2). The names sound alike; the mechanisms are different.

## Exercise

Goal: see the compiler pick overloads, trigger an ambiguous call, and confirm that return-type-only differences are rejected.

1. Create `20260513/day08_ex.cpp`. Add `#include <iostream>` and `#include <cstdint>`. Write three overloads of `describe` before `main`:
   - `void describe(int32_t x)` — prints `"int: "` followed by `x`.
   - `void describe(double x)` — prints `"dbl: "` followed by `x`.
   - `void describe(int32_t a, int32_t b)` — prints `"pair: "` followed by `a` and `b`.

2. In `main`, call all three: `describe(7)`, `describe(3.14)`, `describe(10, 20)`. Predict the output before running.

3. Add two more overloads: `void describe(double a, int32_t b)` and `void describe(int32_t a, double b)`. Then add the call `describe(1, 2)` in `main`. Compile and read the error — it should say "ambiguous" and name at least two candidates. Then remove just that one call (keep the two new overloads) and re-compile to verify it compiles cleanly.

4. At the top of the file (outside `main`), add these two functions:
   ```cpp
   int32_t compute(double x) { return 0; }
   double  compute(double x) { return x * 2.0; }
   ```
   These have the same name and the same parameter list — only the return type differs. Compile and read the error, which should mention redefinition or a conflicting declaration. Then remove those two lines before continuing.

5. Compile and run the final (error-free) program:
   ```bash
   g++ -std=c++17 -Wall -Wextra day08_ex.cpp -o day08_ex
   ./day08_ex
   ```
   Self-check: `describe(7)` prints `int: 7`, `describe(3.14)` prints `dbl: 3.14`, `describe(10, 20)` prints `pair: 10 20`; the ambiguous-call error in step 3 names two candidate overloads; the error in step 4 mentions redefinition or conflicting return types.

## Tomorrow's Preview

**Day 9 — Default Arguments.** You saw today that providing a second parameter creates a separate overload. Tomorrow you will learn about default arguments — a way to write *one* function whose later parameters have pre-set values — which is sometimes a cleaner alternative to overloading when extra parameters are optional.

## Links

- [Roadmap](../cpp-roadmap.md)
- Previous: [Day 7 — Functions: Pass by Value, Ref, Const Ref](2026-05-06-day-07-functions-pass-by-value-ref-const-ref.md)
