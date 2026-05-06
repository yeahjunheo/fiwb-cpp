---
date: 2026-05-06
day: 7
phase: 01-foundations
tags: [cpp, quant, functions, parameter-passing]
---

# Day 7 — Functions: Pass by Value, Ref, Const Ref

> **Why this matters:** You know from Day 6 that every function call gets its own stack frame. Today you learn what happens to the *arguments* you hand to a function — are they copied into that frame, or does the function share the original variable directly? The answer depends on how you declare the parameter, and getting it wrong either wastes CPU time (an unnecessary copy) or breaks your logic (a change that should have propagated but didn't). This decision appears in every function signature you will ever write.

## Concept

### A quick reminder: the stack frame

When `main` calls a function `process`, the CPU pushes a new stack frame onto the stack. That frame holds all of `process`'s local variables, including its **parameters** — the named variables listed in the function signature that receive the arguments you passed in.

The question is: what exactly goes into those parameter slots?

### Pass by value — the function gets its own copy

When you declare a parameter *without* `&`, you are passing by **value**. The argument is copied into the function's parameter variable. The function then works with its own private copy; the original variable in the caller is untouched, no matter what happens inside the function.

```cpp
void apply_fee(double price, double fee) {
    price = price - fee;   // modifies only the local copy
}
```

In `main`:

```cpp
double market_price = 100.0;
apply_fee(market_price, 0.25);
// market_price is still 100.0 — apply_fee worked on a copy
```

Pass by value is **safe** — the caller's data can never be accidentally changed — but it can be **expensive** for large objects, because the copy takes time and memory proportional to the size of the object.

For small types like `int32_t` or `double` (4 or 8 bytes), the cost of copying is negligible. When objects grow larger — a type holding dozens of fields, for example — that cost starts to matter.

### Pass by reference — the function works on the original

When you add `&` after the type name in a parameter declaration, the function receives a **reference** to the original variable rather than a copy. Any modification the function makes through that reference changes the original directly.

```cpp
void apply_fee_inplace(double& price, double fee) {
    price = price - fee;   // modifies the ORIGINAL variable in the caller
}
```

In `main`:

```cpp
double market_price = 100.0;
apply_fee_inplace(market_price, 0.25);
// market_price is now 99.75
```

A reference parameter is an alias — a different name for the same memory location. No copy is made. Under the hood, the compiler passes the address of the original variable (8 bytes on a 64-bit machine), regardless of how large the original is. This makes passing by reference fast even for very large objects.

The trade-off: the function *can* modify the original, intentionally or not. If you want the speed benefit without the modification risk, you need the third option.

> **Key idea:** Pass by reference avoids the copy and lets the function modify the original. If you only want the first benefit but not the second, add `const`.

### Pass by const reference — fast, read-only access

Adding `const` before a reference parameter tells the compiler: "give me a reference to the original (no copy), but I promise not to modify it." You write this as `const double&` or `const SomeType&`.

```cpp
double compute_spread(const double& bid, const double& ask) {
    // bid = 0.0;   // COMPILE ERROR — const prevents modification
    return ask - bid;
}
```

`const&` is the most commonly used parameter style in real C++ code for anything larger than a few bytes. It combines two desirable properties:

- **No copy** — just like a plain reference, fast regardless of size.
- **No accidental modification** — the compiler enforces the promise.

There is one additional benefit: a `const&` parameter can accept a **temporary value** — a literal like `3.14` or the result of an expression — while a plain `&` cannot. A temporary has no persistent address you could form a mutable reference to, so the compiler rejects it. `const&` is allowed because you promised not to modify it, and the language extends the temporary's lifetime to match the reference.

```cpp
double result = compute_spread(99.5, 100.25);  // 99.5 and 100.25 are temporaries
                                                // OK because parameters are const&
```

If the parameters were plain `double&`, this call would be a compile error.

### When to use which

| Situation | Use |
|---|---|
| Small, cheap type (`int`, `double`, pointer) — read-only | By value (simple and fast) |
| Small, cheap type — need to modify the original | By reference (`&`) |
| Large object — read-only | By const reference (`const&`) — avoids the copy |
| Large object — need to modify the original | By reference (`&`) |

A practical rule you can apply right now: **for types bigger than a pointer (8 bytes), default to `const&` unless you need to modify the original.** As you encounter larger types — strings, vectors, custom classes — this habit will keep your code efficient without extra thought.

## Code Example

Create `20260506/day07.cpp`:

```cpp
// day07.cpp — pass by value, reference, and const reference side by side

#include <iostream>
#include <cstdint>

// Pass by VALUE: changes to x stay inside the function
void zero_out_value(int32_t x) {
    x = 0;   // local copy only
    std::cout << "  inside zero_out_value, x = " << x << "\n";
}

// Pass by REFERENCE: changes to x reach the caller
void zero_out_ref(int32_t& x) {
    x = 0;   // modifies the original
    std::cout << "  inside zero_out_ref,   x = " << x << "\n";
}

// Pass by CONST REFERENCE: read-only, no copy
double compute_spread(const double& bid, const double& ask) {
    // bid = 0.0;  // would be a compile error
    return ask - bid;
}

int main() {
    // --- Pass by value ---
    int32_t count = 10;
    std::cout << "Before zero_out_value: count = " << count << "\n";
    zero_out_value(count);
    std::cout << "After  zero_out_value: count = " << count << "\n";
    std::cout << "\n";

    // --- Pass by reference ---
    int32_t total = 10;
    std::cout << "Before zero_out_ref:   total = " << total << "\n";
    zero_out_ref(total);
    std::cout << "After  zero_out_ref:   total = " << total << "\n";
    std::cout << "\n";

    // --- Pass by const reference ---
    double bid = 99.75;
    double ask = 100.25;
    double spread = compute_spread(bid, ask);
    std::cout << "bid = " << bid << "  ask = " << ask << "\n";
    std::cout << "spread = " << spread << "\n";
    std::cout << "\n";

    // --- const& also accepts temporary values; plain & would be a compile error ---
    double s2 = compute_spread(99.50, 100.00);   // literals are fine with const&
    std::cout << "spread from literals = " << s2 << "\n";

    return 0;
}
```

Compile and run:

```bash
g++ -std=c++17 -Wall -Wextra day07.cpp -o day07
./day07
```

Expected output:

```
Before zero_out_value: count = 10
  inside zero_out_value, x = 0
After  zero_out_value: count = 10

Before zero_out_ref:   total = 10
  inside zero_out_ref,   x = 0
After  zero_out_ref:   total = 0

bid = 99.75  ask = 100.25
spread = 0.5

spread from literals = 0.5
```

Notice that `count` is unchanged after `zero_out_value` — the function zeroed its own copy. Meanwhile `total` becomes 0 after `zero_out_ref` because the function held a reference to `total`'s actual memory location. The `bid` and `ask` variables are unchanged after `compute_spread` because `const&` only permits reading.

## Quant Application

A **market data message** is a data packet broadcast by an exchange describing one event — a new order arriving, an existing order being cancelled, or a trade executing at a given price. On a busy exchange during peak hours these arrive hundreds of thousands of times per second, each carrying dozens of fields: instrument identifier, price, quantity, timestamp, sequence number. In a C++ trading system, a function that processes each incoming message must receive that message object somehow. If it receives it by value, the program makes a full copy of every field for every single message. At high throughput, those copies accumulate and waste both CPU time and memory bandwidth. The standard practice is to pass market data messages — and all other large, read-only structures — by `const&`: one pointer's worth of overhead, zero copying.

The **order book** is the central data structure in trading: a live list of all resting buy orders (the **bids** — prices buyers are willing to pay) and sell orders (the **asks** — prices sellers will accept) for a security, organized by price level. The difference between the best (highest) bid and the best (lowest) ask is called the **spread** — it is the minimum cost of immediately buying and then selling the same security. A function that only *reads* the order book — say, to compute the current spread — should take it by `const&`. A function that *updates* the book when a new order arrives should take it by plain `&`. This choice appears in every function signature in a trading system, and senior developers treat `const&` for large read-only parameters as a rule rather than a suggestion.

High-frequency trading (HFT) — trading that executes orders in single-digit microseconds, where a one-microsecond delay has real monetary cost — makes this discipline non-negotiable. In the **hot path** (the code that runs once for every incoming price update, called a **tick**), an unnecessary copy of a large structure can add dozens of nanoseconds of latency. Code reviewers at HFT firms will flag `void handle(MarketData msg)` as a correctness-class bug and require `void handle(const MarketData& msg)`. The habit you are building today — reach for `const&` on large read-only parameters — is exactly the habit that distinguishes latency-conscious C++ from casual C++.

## Pitfalls

- **Forgetting `&` and wondering why the function has no effect.** You write a function that is supposed to modify its argument, run it, and the original is unchanged. The cause is almost always a missing `&` on the parameter. The symptom is distinctive: the function runs without error, but nothing changes in the caller. Add `&` to the parameter type to fix it.

- **Returning a reference to a local variable.** A function's local variables live in its own stack frame, which is destroyed when the function returns. Returning a `&` or `const&` to a local produces a **dangling reference** — a reference to memory that no longer belongs to your program. The compiler often warns about this with `-Wall`, but not always. Never return `&` to a local variable.

- **Passing a large object by value by accident.** Writing `void process(BigObject obj)` when you meant `void process(const BigObject& obj)` compiles silently and correctly — there is no warning. The cost only shows up at runtime as unexpected slowness. Build the habit of reviewing parameter types for any object larger than a few basic fields.

- **Modifying a by-value parameter and expecting the change to persist.** Inside the function, a by-value parameter is just a local variable. Changing it is fine for local computation (reusing the variable as a temporary), but those changes vanish at `return`. They never reach the caller.

- **Trying to pass a literal or computed result to a non-const reference parameter.** `void f(double& x)` cannot be called as `f(3.14)` because `3.14` is a temporary with no persistent address to form a mutable reference to. The fix is to change the parameter to `const double&` if you do not need to modify it, or to store the value in a named variable first if you do.

- **Confusing `const double& x` with `double const& x`.** These are identical in meaning — both declare a read-only reference to a `double`. The first form (`const T&`) is the idiomatic style you will see in most code. The second is valid but rare. Both are correct; don't let either form confuse you when reading someone else's code.

## Exercise

Goal: observe with your own eyes that pass-by-value leaves the caller's variable untouched, pass-by-reference changes it, and that `const&` is enforced at compile time.

1. Create `20260506/day07_ex.cpp`. Add `#include <iostream>` and `#include <cstdint>`. Write two functions before `main`:
   - `void add_ten_value(int32_t n)` — sets `n = n + 10;` and prints `n` inside.
   - `void add_ten_ref(int32_t& n)` — sets `n = n + 10;` and prints `n` inside.

2. In `main`, declare `int32_t score = 5;`. Call `add_ten_value(score)`, then print `score`. Call `add_ten_ref(score)`, then print `score` again. **Before running**, predict what each line will print.

3. Write a third function `double midpoint(const double& a, const double& b)` that returns `(a + b) / 2.0`. Call it from `main` with `double bid = 99.0;` and `double ask = 101.0;` and print the result.

4. Temporarily add `a = 0.0;` inside `midpoint`. Try to compile and read the error message — it will say something like "assignment of read-only reference." Then remove that line before continuing.

5. Compile and run the final (error-free) program:
   ```bash
   g++ -std=c++17 -Wall -Wextra day07_ex.cpp -o day07_ex
   ./day07_ex
   ```
   Self-check: `score` should be 5 after `add_ten_value` and 15 after `add_ten_ref`; `midpoint(99.0, 101.0)` should print `100`; the compiler error in step 4 should mention `const` or "read-only."

## Tomorrow's Preview

**Day 8 — Function Overloading.** You have written functions that take specific parameter types. Tomorrow you will see that C++ lets you define multiple functions with the same name but different parameter types — the compiler automatically picks the right version based on what arguments you pass in.

## Links

- [Roadmap](../cpp-roadmap.md)
- Previous: [Day 6 — Stack vs Heap Basics](2026-05-03-day-06-stack-vs-heap-basics.md)
