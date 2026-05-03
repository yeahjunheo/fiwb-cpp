---
date: 2026-05-03
day: 5
phase: 01-foundations
tags: [cpp, quant, pointers, memory, pointer-arithmetic]
---

# Day 5 — Pointer Arithmetic

> **Why this matters:** Yesterday you learned that a pointer holds a numeric address and that you can dereference it to read or write the value there. Today you learn to *move* that address — forward and backward through a contiguous block of memory. This is how C++ traverses arrays without copying data, how network parsers walk through raw byte streams, and how trading systems process incoming price updates at full speed. It also demystifies array indexing: `arr[i]` is literally defined as pointer arithmetic under the hood.

## Concept

Recall from Day 4 that every variable lives at a specific address — a number the CPU uses to locate the bytes for that variable. A pointer stores that number.

Now picture memory as a long row of mailboxes. Each mailbox holds one byte. An `int32_t` takes four consecutive mailboxes. If the first mailbox for a variable is numbered 1000, the variable occupies boxes 1000, 1001, 1002, and 1003. The next `int32_t` in memory would start at box 1004.

This structure — multiple values of the same type packed back-to-back — is called a **C-style array** (an array is a fixed block of N elements of the same type stored consecutively in memory; we cover arrays in depth on Day 14). When elements are contiguous, moving from one to the next is just adding the size of one element to the current address. Pointer arithmetic is the language mechanism that does this automatically.

> **Key idea:** When you add an integer `n` to a pointer `ptr`, the result is a pointer that is `n * sizeof(*ptr)` bytes ahead. The compiler handles the byte multiplication for you — you think in elements, not bytes.

So if `p` holds address 1000 and `*p` is `int32_t` (4 bytes):

```
p + 0  →  address 1000  (element 0)
p + 1  →  address 1004  (element 1)
p + 2  →  address 1008  (element 2)
p + 3  →  address 1012  (element 3)
```

Adding 1 to a `double*` moves 8 bytes. Adding 1 to a `char*` moves 1 byte. The step size is always `sizeof` of the pointed-to type, and the compiler knows it from the pointer's declared type.

### Adding and subtracting integers

```cpp
int32_t* p = /* some address */;
int32_t* next = p + 1;   // one element forward
int32_t* prev = p - 1;   // one element backward
```

Neither of these reads or writes any memory — they just compute a new address. Nothing happens until you dereference with `*`.

### Increment and decrement

```cpp
++p;   // advance p by one element; returns the new (advanced) value of p
--p;   // retreat p by one element
p++;   // advance p by one element; returns the old value of p before advancing
```

The difference between `++p` and `p++` is what value is handed back to the rest of the expression. `++p` (pre-increment) gives you the new, already-advanced pointer. `p++` (post-increment) gives you the old pointer, then advances. In a simple traversal loop this rarely matters, but in expressions like `*p++` it does: that reads the current element and then advances, which is a common idiom you will encounter in real code. For now, use `++p` in loops to make the intent explicit.

### The subscript–pointer equivalence

Here is one of the most important identities in all of C++:

```
ptr[i]  ≡  *(ptr + i)
```

This is not a convenience — it is the formal *definition* of the subscript operator. `ptr[i]` compiles to exactly the same machine code as `*(ptr + i)`. This means:

- Array indexing is pointer arithmetic in disguise.
- You can use `[]` on any pointer, not just an array-name pointer.
- Understanding pointer arithmetic means you already know how array indexing works at the machine level.

### Pointer subtraction

When two pointers both point into the same block of memory, subtracting them gives you the number of elements between them:

```cpp
ptrdiff_t gap = ptr_b - ptr_a;
```

The result type is **`ptrdiff_t`** — a signed integer type defined in `<cstddef>` specifically for pointer differences. It is signed because `ptr_b - ptr_a` is negative when `ptr_a` is ahead of `ptr_b`. On a 64-bit machine `ptrdiff_t` is typically 64 bits wide.

### The one-past-the-end rule

For an array of 5 elements (indices 0–4), the valid element addresses are at positions 0 through 4. There is one additional address that is explicitly legal to *form* — but not legal to *dereference* — the address just beyond the last element, called **one-past-the-end**:

```cpp
int32_t arr[5] = {10, 20, 30, 40, 50};
int32_t* end = arr + 5;   // legal to hold; do NOT write *end
```

`end` does not point at a valid `int32_t`. Reading or writing `*end` is undefined behavior. But *holding* that address and *comparing* other pointers against it is perfectly legal. This gives you a clean loop pattern:

```cpp
int32_t* cur = arr;
while (cur != end) {
    std::cout << *cur << " ";   // safe: cur always points to a real element
    ++cur;                       // when cur reaches arr+5, the loop stops
}
```

This "start pointer + one-past-the-end sentinel" pattern is the foundation of every iterator-based loop in the standard library. You will see it again on Day 72 when we cover iterators.

### Undefined behavior boundaries

Pointer arithmetic is only well-defined within the same contiguous array, from the first element through one-past-the-end. Stepping further is undefined behavior — the program may crash, silently produce wrong answers, or appear to work with no guarantees:

```cpp
int32_t arr[5] = {10, 20, 30, 40, 50};
int32_t  x = *(arr + 5);   // UB: dereferencing one-past-the-end
int32_t  y = *(arr - 1);   // UB: before the start of the array
int32_t* z =  arr + 6;     // UB: even forming this address is UB (two past the end)
```

Subtracting two pointers that belong to *different* arrays is also undefined behavior, even if the raw numeric addresses happen to be close together.

## Code Example

A brief note on the array syntax used below: `int32_t prices[5]` declares a C-style array of five `int32_t` values stored consecutively. When you write the bare name `prices` (without brackets), C++ automatically gives you a pointer to `prices[0]`. Day 14 covers arrays fully; for today that one fact is all you need.

Create `pointer_arith.cpp`:

```cpp
// pointer_arith.cpp — pointer arithmetic fundamentals

#include <iostream>
#include <cstdint>
#include <cstddef>   // ptrdiff_t

int main() {
    int32_t prices[5] = {100, 101, 102, 103, 104};

    // 'prices' used as a pointer gives the address of prices[0]
    int32_t* p = prices;

    // --- ptr + n and *(ptr + n) ---
    std::cout << "Element 0: " << *p       << "\n";   // 100
    std::cout << "Element 1: " << *(p + 1) << "\n";   // 101
    std::cout << "Element 4: " << *(p + 4) << "\n";   // 104

    // --- subscript is defined as pointer arithmetic ---
    std::cout << "p[2]   = " << p[2]     << "\n";   // 102
    std::cout << "*(p+2) = " << *(p + 2) << "\n";   // 102 — identical result

    // --- traversal with ++cur ---
    std::cout << "\nAll prices: ";
    int32_t* end = prices + 5;   // one-past-the-end sentinel; never dereferenced
    int32_t* cur = prices;
    while (cur != end) {
        std::cout << *cur << " ";
        ++cur;
    }
    std::cout << "\n";

    // --- pointer subtraction ---
    int32_t* first = prices;
    int32_t* last  = prices + 4;   // points to prices[4] = 104
    ptrdiff_t dist = last - first; // result: 4 (elements, not bytes)
    std::cout << "Elements from first to last: " << dist << "\n";

    // --- sizeof confirms the per-element step size ---
    std::cout << "sizeof(int32_t) = " << sizeof(int32_t) << " bytes\n";
    std::cout << "4 elements span  = " << 4 * sizeof(int32_t) << " bytes\n";

    return 0;
}
```

Compile and run:

```bash
g++ -std=c++17 -Wall -Wextra pointer_arith.cpp -o pointer_arith
./pointer_arith
```

Expected output:

```
Element 0: 100
Element 1: 101
Element 4: 104
p[2]   = 102
*(p+2) = 102

All prices: 100 101 102 103 104
Elements from first to last: 4
sizeof(int32_t) = 4 bytes
4 elements span  = 16 bytes
```

Notice that `dist` is 4 (elements), while the raw byte span is 16 (4 elements × 4 bytes each). Pointer subtraction gives you element counts — the unit a human programmer wants — while the byte math happens automatically inside the CPU.

## Quant Application

In a trading system, **market data** (the stream of price and trade events broadcast by an exchange) arrives over the network as a dense binary byte stream. Each **tick** (a single price update from an exchange, the smallest unit of market data) is a compact binary record — perhaps 16 bytes — packed into a receive buffer. The parser that reads it maintains a `char*` pointer and advances it field by field: `ptr += sizeof(uint16_t)` to step over a 2-byte field, `ptr += sizeof(int32_t)` to step over a 4-byte price. This is pointer arithmetic at its most direct — the code walks through memory one field at a time, with zero copying and zero heap allocation. Understanding that `ptr + n` moves by `n * sizeof` bytes is what makes this both correct and fast.

Many trading strategies compute **signals** — numeric values derived from recent prices used to decide whether to buy or sell. A simple example is a moving average: the average of the last N prices, used as a basic trend indicator. A typical implementation stores the last hundred or so prices in a contiguous `int32_t` array and computes the sum by walking a pointer from the buffer's start to its end, accumulating `*cur` at each step. The fact that this is a sequential walk over contiguous memory is significant for performance: modern CPUs fetch memory from RAM in 64-byte chunks (called **cache lines**, covered on Day 114), and linear pointer traversal loads those chunks in exactly the right order. A strategy that accesses prices non-sequentially would pay a much higher memory-latency penalty.

Pointer arithmetic also shapes the internal design of **order books** — the complete list of active buy and sell orders for a security at each price level. One common design packs all resting orders at a single price level into a contiguous array. The **matching engine** (the component of an exchange that pairs buy and sell orders) walks a pointer from the front of that array toward the end, filling each waiting order in turn. The one-past-the-end sentinel makes the "are we done?" check trivial: `while (order_ptr != level_end)`. There is no index variable to manage and no off-by-one risk. When we build a basic matching engine starting on Day 134, this is exactly the traversal pattern you will write.

## Pitfalls

- **Dereferencing one-past-the-end.** Forming `arr + N` for an N-element array is legal; writing `*(arr + N)` is undefined behavior. The compiler will not warn you — you must track the bound yourself.

- **Going before the array start.** `arr - 1` is undefined behavior — there is no "one-before-the-start" equivalent to one-past-the-end. The only special-case boundary is at the far end.

- **Confusing `*p++` and `*++p`.** The expression `*p++` dereferences `p` and then advances it (post-increment). The expression `*++p` advances `p` first and then dereferences it. Both are legal; they access different elements. When learning, split these onto two lines for clarity.

- **Subtracting unrelated pointers.** `ptr_a - ptr_b` is only meaningful when both pointers point into the same array. Subtracting pointers from two different variables is undefined behavior, even if the numbers happen to produce a plausible-looking result.

- **Pointer arithmetic on `void*` is a compile error.** `void*` has no associated type, so the compiler has no `sizeof` step to use. You must cast to a concrete pointer type first.

- **"The next address is address + 1."** A common beginner mistake: thinking pointer addition always moves one byte. For `int32_t*` the step is 4 bytes; for `double*` it is 8 bytes. The step is always `sizeof(*ptr)`, chosen automatically by the compiler from the pointer's type.

## Exercise

Goal: build hands-on intuition by traversing an array using only pointer arithmetic — no `[]` subscript allowed.

1. Create a new file at `20260503/day05.cpp`. Include `<iostream>`, `<cstdint>`, and `<cstddef>`. Write `main()` returning `0`. Inside `main`, declare:
   ```cpp
   int32_t ticks[5] = {200, 198, 201, 199, 202};
   ```
   (These represent five consecutive price ticks from a market data feed.)

2. Forward traversal using only `*`, `+`, and `++cur`: declare `int32_t* end = ticks + 5;` and `int32_t* cur = ticks;`. Write a `while` loop that prints `*cur`, then does `++cur`, stopping when `cur == end`. Expected output: `200 198 201 199 202`.

3. Reverse traversal: start a counter `int32_t i = 4;` and loop `while (i >= 0)`, printing `*(ticks + i)` and then `--i`. This uses pointer arithmetic (`ticks + i`) without risking stepping before the array start. Expected output: `202 199 201 198 200`.

4. Find the minimum tick using a pointer loop: set `int32_t min_val = *ticks;`, then walk `cur` from `ticks` to `end`, updating `min_val = *cur` whenever `*cur < min_val`. Print `min_val` after the loop. Expected: `198`.

5. Compute and print the pointer distance:
   ```cpp
   int32_t* first = ticks;
   int32_t* last  = ticks + 4;
   ptrdiff_t dist = last - first;
   std::cout << "Elements spanned: " << dist << "\n";
   ```

   Compile:
   ```bash
   g++ -std=c++17 -Wall -Wextra day05.cpp -o day05
   ./day05
   ```

Self-check: forward prints `200 198 201 199 202`, reverse prints `202 199 201 198 200`, minimum is `198`, distance is `4`.

## Glossary

- **C-style array** — a fixed-size block of N elements of the same type stored consecutively in memory. The bare array name decays to a pointer to the first element. Covered fully on Day 14.
- **ptrdiff_t** — a signed integer type in `<cstddef>` designed to hold the result of subtracting two pointers. Typically 64 bits on a 64-bit machine.
- **One-past-the-end** — the address immediately after the last element of an array. Legal to form and compare against; illegal to dereference.
- **Tick** — a single price or size update broadcast by an exchange. The smallest unit of market data.
- **Signal** — a numeric value computed from market data that a strategy uses to make trading decisions (e.g., a moving average). Covered in depth in Phase 7.
- **Cache line** — a 64-byte chunk of memory the CPU fetches from RAM in one operation. Sequential pointer traversal uses cache lines efficiently. Covered on Day 114.
- **Matching engine** — the component of an exchange that pairs buy orders with sell orders. Covered starting on Day 134.

## Tomorrow's Preview

**Day 6 — Stack vs Heap Basics.** Today you moved through contiguous memory; tomorrow you will learn the two regions of memory a C++ program uses — the stack and the heap — what makes them different in speed and lifetime, and why that distinction matters every time you declare a variable.

## Links

- [Roadmap](../cpp-roadmap.md)
- Previous: [Day 4 — References vs Pointers](2026-05-01-day-04-references-vs-pointers.md)
