---
date: 2026-05-01
day: 4
phase: 01-foundations
tags: [cpp, quant, references, pointers, memory]
---

# Day 4 — References vs Pointers

> **Why this matters:** You know the types that hold your data (Day 2) and how to make values read-only (Day 3). The next step is understanding how one part of your program can reach into another part's data without making a copy of it. References and pointers are both tools for this. References are simpler and safer; pointers are more flexible but require more care. Both appear in virtually every line of professional C++ code, and knowing when to use each is a skill you will exercise from here to the end of the curriculum.

## Concept

Every variable you declare lives somewhere in your computer's memory. Memory is a giant sequence of bytes, and each byte has a unique number called its **address** — think of addresses like house numbers on a street where each house is one byte. When you write `int32_t price = 100;`, the compiler reserves some bytes in memory and stores 100 there. The address of that storage is a fact you can observe and work with.

C++ gives you two tools to work with addresses:

- A **reference** is an alias — a second name for the same variable. Reading or writing through the reference is identical to reading or writing through the original name.
- A **pointer** is a variable that *stores* an address. Where a reference *is* the other variable, a pointer *holds the location* of another variable.

Both are fundamental to writing efficient C++. Let's look at each in detail.

### References

A reference is declared by placing `&` after the type name:

```cpp
int32_t price = 100;
int32_t& ref = price;   // ref is another name for price
```

Three rules about references that you must know cold:

**1. A reference must be initialized immediately.** You cannot write `int32_t& ref;` and fill it in on the next line — the compiler refuses. A reference must name something from the instant it exists.

**2. A reference cannot be rebound.** Once `ref` is attached to `price`, it stays attached to `price` for its entire lifetime. If you later write `ref = other;`, you are *not* making `ref` point at `other` — you are *copying the value of `other` into `price`*. The reference does not move; it writes through to the original variable.

**3. A reference cannot be null.** There is no such thing as a reference to nothing. If a reference exists, it always names a real variable. This is the key safety advantage over pointers.

Because a reference is an alias, modifying it modifies the original:

```cpp
int32_t price = 100;
int32_t& ref = price;
ref = 200;               // sets price to 200; ref still refers to price
```

### Const references

Combining `const` from Day 3 with a reference gives `const int32_t&` — you can read the value through it but cannot change it:

```cpp
const int32_t& cref = price;   // read-only view of price
// cref = 300;                 // COMPILE ERROR
```

A const reference has one special ability that a plain reference does not: it can bind to a temporary value or a literal, while a plain non-const reference cannot:

```cpp
const int32_t& r = 42;   // legal — const ref extends the temporary's lifetime
int32_t& s = 42;         // COMPILE ERROR — non-const ref cannot bind to a literal
```

This rule matters when you write function parameters on Day 7, where `const double& price` is the standard way to accept a value for reading without copying it.

### Pointers

A pointer is declared by placing `*` after the type name. It holds the *address* of another variable. You obtain that address using the **address-of operator** `&` (same symbol as reference declaration, different context):

```cpp
int32_t price = 100;
int32_t* ptr = &price;   // ptr stores the address of price
```

Notice the two uses of `&`: in `int32_t& ref = price` it creates a reference; in `&price` applied to an existing variable it produces that variable's address. Context determines which meaning applies.

To read or write the value at the address a pointer holds, use the **dereference operator** `*`:

```cpp
int32_t price = 100;
int32_t* ptr = &price;

int32_t value = *ptr;    // reads the value at the address: value == 100
*ptr = 200;              // writes through the pointer: price is now 200
```

Unlike a reference, a pointer:

- **Can be null.** `int32_t* ptr = nullptr;` is a valid pointer that points to nothing. Dereferencing a null pointer — writing `*ptr` when `ptr == nullptr` — is **undefined behavior**: your program will usually crash immediately, and on rare occasions will silently corrupt memory, which is harder to debug. Always check before dereferencing any pointer you did not initialize yourself.

- **Can be reassigned.** You can make a pointer aim at a different variable. `ptr = &other;` changes where the pointer points without touching either variable.

> **Key idea:** A reference must always name a real object and never changes its attachment. A pointer can be null and can be re-aimed. Use references when the thing you want to name always exists; use pointers when it might be absent or might change.

### `nullptr`

`nullptr` (introduced in C++11) is the correct way to write "this pointer points to nothing." Avoid the old-style `NULL` or the integer `0` — `nullptr` has its own dedicated type and produces clearer compiler error messages when misused.

### `const` and pointers

A pointer has two separate components: the address it holds, and the value at that address. `const` can apply to either component or both. The grammar is read right-to-left from the variable name:

| Declaration | Plain English |
|---|---|
| `const int32_t* ptr` | pointer to const — can re-aim ptr, cannot change the value through it |
| `int32_t* const ptr = &x` | const pointer — cannot re-aim ptr, can change the value through it |
| `const int32_t* const ptr = &x` | const pointer to const — neither component changeable |

The one that trips beginners: `const int32_t* ptr` and `int32_t const* ptr` mean exactly the same thing — both put `const` on the value, not the pointer. To make the pointer itself const, `const` goes *after* the `*`.

## Code Example

Create `refs_ptrs.cpp`:

```cpp
// refs_ptrs.cpp
// References and pointers side by side.

#include <iostream>
#include <cstdint>

int main() {
    // ---- REFERENCES ----
    int32_t price = 100;
    int32_t& ref = price;    // ref is another name for price

    std::cout << "price = " << price << ", ref = " << ref << "\n";  // 100, 100

    ref = 200;               // writes through ref — changes price
    std::cout << "After ref=200: price = " << price << "\n";        // 200

    const int32_t& cref = price;
    std::cout << "cref = " << cref << "\n";   // 200
    // cref = 300;           // COMPILE ERROR if uncommented

    // ---- POINTERS ----
    int32_t bid = 99;
    int32_t ask = 101;
    int32_t* ptr = &bid;     // ptr holds the address of bid

    std::cout << "\nbid = " << bid << "\n";
    std::cout << "address stored in ptr: " << ptr << "\n";   // hex address, varies each run
    std::cout << "*ptr (value there) = " << *ptr << "\n";    // 99

    *ptr = 98;               // write through the pointer
    std::cout << "After *ptr=98: bid = " << bid << "\n";     // 98

    ptr = &ask;              // re-aim the pointer
    std::cout << "After re-aim: *ptr = " << *ptr << "\n";    // 101

    // ---- NULLPTR ----
    int32_t* maybe = nullptr;

    if (maybe == nullptr) {
        std::cout << "\nmaybe is null.\n";
    }

    // Safe pattern: always check before dereferencing
    if (maybe != nullptr) {
        *maybe = 5;
    } else {
        std::cout << "Pointer null -- skipping dereference.\n";
    }

    // ---- POINTER TO CONST ----
    const int32_t* cptr = &bid;   // can read bid, cannot write through cptr
    std::cout << "\n*cptr = " << *cptr << "\n";   // 98
    // *cptr = 50;               // COMPILE ERROR if uncommented

    return 0;
}
```

Compile and run:

```bash
g++ -std=c++17 -Wall -Wextra refs_ptrs.cpp -o refs_ptrs
./refs_ptrs
```

Expected output (the hex address varies every run — that is normal):

```
price = 100, ref = 100
After ref=200: price = 200
cref = 200

bid = 99
address stored in ptr: 0x7ffd4a3b1e2c
*ptr (value there) = 99
After *ptr=98: bid = 98
After re-aim: *ptr = 101

maybe is null.
Pointer null -- skipping dereference.

*cptr = 98
```

Two things to notice. First, printing `ptr` directly shows the actual memory address of `bid` in hexadecimal — this is the raw number the CPU uses to locate that variable. The address changes on every run because the operating system places your program at a different location each time it starts (a security technique called address space layout randomization). Second, after re-aiming `ptr = &ask`, the value of `bid` is unchanged — re-aiming a pointer does not touch either variable; it only changes what address the pointer remembers.

## Quant Application

In trading systems, performance is dominated by how fast **market data** (the stream of price and trade events broadcast by an exchange) can flow through the program. Processing tens of millions of these events per second is routine in HFT (high-frequency trading — strategies where execution speed measured in microseconds determines profitability). One of the first tools that helps is avoiding unnecessary data copies: when a function needs to work with a large record, giving it a reference to the original means only one copy of that record ever exists in memory. The exact syntax for this arrives on Day 7, but references are what make that pattern possible. Every senior engineer on a trading desk writes `const MarketData&` in function parameters almost reflexively — you are learning the building block that makes that idiom work.

The **order book** (the complete list of active buy orders and sell orders for a security, sorted by price level) is the most important data structure in any matching engine. Within each price level there is typically a chain of individual orders waiting to be filled. Internally these chains are linked lists — each order record contains a pointer to the next order at the same level, and the last order's pointer is `nullptr` to signal the end. Checking `next_order != nullptr` before following the link is one of the most common patterns in order book code. `nullptr` here is not just a convention; it is the clean, type-safe sentinel that requires no magic number and produces an obvious check. You will write this yourself in Phase 7 (Day 134) when you build a basic matching engine.

Pointers also model **optional fields** throughout financial data. A FIX message (FIX — Financial Information eXchange, the dominant protocol for transmitting orders and market data between brokers and exchanges) may or may not include a "last trade price" field: before the first trade of the session, that field is genuinely absent. One natural C++ representation is `const double* last_trade_price = nullptr;` — null means the field was not present, a valid non-null pointer means it was. This is cleaner than a magic sentinel like `-1.0` and the intent is self-documenting. Day 54 will introduce `std::optional`, a type-safe version of this pattern built into the standard library, but understanding pointers first makes its design immediately obvious.

## Pitfalls

- **Dangling references.** If a reference outlives the variable it names, you have undefined behavior — the reference points into memory that no longer belongs to your variable. The classic case is returning a reference to a local variable from a function (functions with return values will be covered fully on Day 7). GCC with `-Wall` warns: "reference to local variable returned." Take the warning seriously.

- **Dereferencing a null pointer.** Always check `ptr != nullptr` before writing `*ptr`. A null dereference causes an immediate crash on most systems (a SIGSEGV signal on Linux). On the rare occasion it does not crash immediately, it silently corrupts memory — which is far harder to debug after the fact.

- **Assignment to a reference writes through, not rebinds.** `ref = other_var;` does not make `ref` start pointing at `other_var`. It copies the value of `other_var` into the original variable `ref` was attached to. Beginners expecting rebinding get a subtle bug where data is silently overwritten. If you need something that can be re-aimed, use a pointer.

- **`int* a, b;` declares one pointer and one int.** The `*` binds to the variable name, not to the type. `b` here is a plain `int32_t`, not a pointer. Prefer one declaration per line to avoid this trap entirely.

- **Reading `const int32_t*` as "const pointer."** It is a "pointer to const" — the `const` applies to the value being pointed at, not to the pointer itself. The pointer is freely re-aimable. To make the pointer constant, write `int32_t* const ptr = &x;` with `const` after the `*`.

- **Using `NULL` or `0` instead of `nullptr`.** `NULL` is a C macro that expands to the integer `0`. In some overload situations this picks the wrong function because it looks like an int. `nullptr` is a proper C++ null pointer literal with its own type. Always write `nullptr` in C++ code.

## Exercise

Goal: build intuition for the reference/pointer distinction by writing both and observing the difference between "write-through" and "re-aim."

1. Create a new folder `20260501` and a file `day04.cpp`. Add `#include <iostream>` and `#include <cstdint>` at the top. Write `main()` returning `0`.

2. Inside `main`, declare these variables and print all three:
   ```cpp
   int32_t a = 10;
   int32_t b = 20;
   int32_t& refa = a;   // reference to a
   int32_t* ptr  = &a;  // pointer to a
   ```
   Compile and run:
   ```bash
   g++ -std=c++17 -Wall -Wextra day04.cpp -o day04
   ./day04
   ```
   `a`, `refa`, and `*ptr` should all print `10`.

3. Write `refa = b;` and then print `a`, `refa`, and `b` again. Notice that `a` changes to `20` — the reference wrote through to `a`. The reference is still attached to `a`, not to `b`. Verify by printing both addresses:
   ```cpp
   std::cout << "&a = " << &a << ", &refa = " << &refa << "\n";
   ```
   They must be identical because `a` and `refa` are the same variable.

4. Now re-aim the pointer: write `ptr = &b;` and print `*ptr`. It should print `20` (the current value of `b`). Also print `&b` and `ptr` and confirm they match. This is the key difference — the pointer changed its target; the reference did not.

5. Add this null-safe block and confirm it runs without crashing:
   ```cpp
   int32_t* maybe = nullptr;
   if (maybe != nullptr) {
       std::cout << *maybe << "\n";
   } else {
       std::cout << "null -- safe skip\n";
   }
   ```
   Then try removing the `if` check and just writing `std::cout << *maybe << "\n";`. This should crash (or produce garbage). Restore the safe version when done.

6. Self-check before moving on:
   - "I want a second name for a variable that I can read but not change — what type?" (`const int32_t&`)
   - "I want something that starts as null and gets filled in later — reference or pointer?" (pointer)
   - "After `refa = b;`, does `refa` now refer to `b`?" (No — it still refers to `a`, and `a`'s value was updated to `b`'s value.)

## Glossary

- **Address** — the numeric location of a variable's bytes in memory. The operator `&var` yields the address of `var`.
- **Dereference** — following a pointer to access the value at its address. Written `*ptr`.
- **Undefined behavior** — code whose outcome the C++ standard does not define. It may crash, produce garbage results, or appear to work correctly with no guarantees. Treated as a hard error throughout this curriculum.
- **nullptr** — the C++ null pointer literal, meaning "this pointer points to nothing." Dereferencing it is undefined behavior.
- **Order book** — the list of all active buy and sell orders for a security, organized by price level. Covered in depth starting Day 134.
- **Market data** — the stream of price and trade events broadcast by an exchange. Referenced throughout Phase 7.
- **HFT** — high-frequency trading; strategies where execution latency in microseconds determines profitability. Covered systematically in Days 159–165.
- **FIX protocol** — the dominant electronic messaging standard for order transmission between financial firms. Covered on Day 142.

## Tomorrow's Preview

**Day 5 — Pointer Arithmetic.** Today you learned that a pointer holds an address. Tomorrow you will learn how to move that address forward and backward — the foundation for traversing arrays and raw memory buffers, and the basis for how C++ processes historical price data efficiently.

## Links

- [Roadmap](../cpp-roadmap.md)
- Previous: [Day 3 — `const`, `constexpr`, `mutable`](2026-04-30-day-03-const-constexpr-mutable.md)
