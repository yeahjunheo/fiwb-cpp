---
date: 2026-05-04
day: 6
phase: 01-foundations
tags: [cpp, quant, memory, stack, heap]
---

# Day 6 — Stack vs Heap Basics

> **Why this matters:** Every variable you declare lives somewhere in memory. So far the variables you've seen — locals inside functions, parameters — all live in a region called the *stack*. But there's a second region, the *heap*, where memory can be requested at runtime and can outlast the scope that created it. Understanding the difference explains why some programs crash with "stack overflow," why heap allocation is slower than stack allocation, and why high-frequency trading systems go to extraordinary lengths to avoid allocating heap memory during live trading.

## Concept

### Two regions of memory

A running C++ program uses several distinct memory regions. The two you'll interact with most are:

- **The stack** — a fixed-size region managed automatically by the CPU and compiler. Local variables and function parameters live here. Memory is allocated when a scope opens and freed when it closes — automatically, with no code from you.
- **The heap** (also called the *free store*) — a much larger region managed manually by the programmer using `new` and `delete`. Memory here persists until you explicitly release it.

Think of the stack as a stack of cafeteria trays: each function call adds a new tray on top (holding its local variables), and when the function returns, that tray is removed. The heap is more like a warehouse with numbered shelves: you request a shelf, receive its location as a number (a pointer), use the shelf as long as you need, and eventually return it to the pool.

### The stack: automatic lifetime

When you declare a local variable inside a function or block, C++ places it on the stack automatically:

```cpp
void foo() {
    int32_t x = 42;   // placed on the stack when foo() is called
    double  y = 3.14; // also on the stack
}   // x and y are destroyed here — stack memory is recovered automatically
```

This "automatic" behavior is called **automatic storage duration**. You never write cleanup code — the compiler generates it. Internally, the CPU tracks the current top of the stack using a register called the **stack pointer**; allocating a local variable is as simple as subtracting from that register (one CPU instruction).

Stack allocation is therefore extremely fast — a handful of CPU cycles regardless of how many variables you declare. There is no searching for free space, no coordination with the operating system.

The trade-off is size. The stack is typically limited to **8 megabytes** on Linux. A `double` array of one million elements would need 8 MB — exhausting the entire stack and crashing the program with a **stack overflow** (when a program tries to use more stack space than the operating system permits, it receives a signal and terminates).

### The heap: dynamic lifetime

Sometimes you need memory that:

- is too large for the stack,
- has a lifetime that doesn't fit neatly into a single scope, or
- whose size is unknown until the program is actually running.

For these cases, C++ provides `new` and `delete`.

**`new` allocates memory on the heap and returns a pointer to it:**

```cpp
int32_t* p = new int32_t;   // allocate space for one int32_t on the heap
*p = 99;                     // store a value there — same dereference you learned on Day 4
```

**`delete` releases the memory back to the heap:**

```cpp
delete p;      // the int32_t at *p is destroyed; heap space is returned
p = nullptr;   // best practice: null the pointer immediately after deleting
```

After `delete p`, the pointer `p` still holds the old numeric address, but that address is no longer valid — the memory has been returned to the heap's free pool. Using `*p` after `delete` is **undefined behavior** — a C++ term meaning "the standard makes no guarantee; the program may crash, silently corrupt data, or appear to work with wrong values." Nulling the pointer right after `delete` turns any accidental future dereference into a clean crash instead of invisible corruption.

If you forget to `delete`, the allocated memory is never returned. This is a **memory leak**: the heap gradually fills with unreachable chunks. In a trading system that processes millions of messages, even a tiny leak per message can exhaust memory and crash the process hours after launch.

### Heap arrays: `new[]` and `delete[]`

To allocate an array of N elements on the heap, use `new[]` and the matching `delete[]`:

```cpp
int32_t* buf = new int32_t[1000];   // allocate 1000 ints on the heap
buf[0] = 10;                         // buf[i] == *(buf + i), exactly as in Day 5
delete[] buf;                         // must use delete[], not delete
buf = nullptr;
```

> **Key idea:** `new` pairs with `delete`; `new[]` pairs with `delete[]`. Mixing them (e.g., using `delete buf` when `buf` came from `new[]`) is undefined behavior. Match the form, always.

### Speed at a glance

| | Stack | Heap |
|---|---|---|
| Allocation cost | ~1 CPU instruction | Hundreds of CPU cycles (free-list search; may involve an OS call) |
| Deallocation cost | Automatic (scope exit) | Explicit `delete`; similar cost to allocation |
| Size limit | ~8 MB (Linux default) | Limited by available RAM and virtual memory |
| Lifetime | Tied to enclosing scope | Until you `delete` |
| Predictability | Constant, always fast | Variable; depends on allocator state |

The cost gap is the reason heap allocation is avoided in the most time-sensitive C++ code. We'll look at this in the Quant Application below.

## Code Example

Create `stack_heap.cpp`:

```cpp
// stack_heap.cpp — stack and heap allocation side by side

#include <iostream>
#include <cstdint>

void demo_stack() {
    int32_t local = 42;         // stack-allocated; automatic lifetime
    std::cout << "Stack variable: " << local
              << "  at address: " << &local << "\n";
    // 'local' is destroyed when demo_stack() returns; no action needed
}

void demo_heap_single() {
    int32_t* p = new int32_t;   // heap-allocated; manual lifetime
    *p = 99;
    std::cout << "Heap variable:  " << *p
              << "  at address: " << p << "\n";
    delete p;      // release the heap memory
    p = nullptr;   // null out so we can't accidentally use the freed address
}

void demo_heap_array() {
    const int32_t N = 5;
    int32_t* prices = new int32_t[N];   // heap array of 5 ints

    for (int32_t i = 0; i < N; ++i) {
        prices[i] = 100 + i;             // fills: 100, 101, 102, 103, 104
    }

    std::cout << "Heap array: ";
    for (int32_t i = 0; i < N; ++i) {
        std::cout << prices[i] << " ";
    }
    std::cout << "\n";

    delete[] prices;   // match new[] with delete[] — not delete
    prices = nullptr;
}

int main() {
    demo_stack();
    demo_heap_single();
    demo_heap_array();
    return 0;
}
```

Compile and run:

```bash
g++ -std=c++17 -Wall -Wextra stack_heap.cpp -o stack_heap
./stack_heap
```

Expected output (addresses will differ on every run and machine):

```
Stack variable: 42  at address: 0x7ffd...
Heap variable:  99  at address: 0x55...
Heap array: 100 101 102 103 104
```

Notice the addresses look different. On x86-64 Linux, the stack lives at high virtual addresses (near `0x7fff...`) while small heap allocations appear much lower (often near `0x55...`). The exact numbers vary per run — the OS chooses locations — but the region difference is consistent.

## Quant Application

In **HFT** (high-frequency trading — strategies that react to market events in microseconds, far faster than any human) latency is the primary constraint. The round-trip time from a market data message arriving at the NIC (network interface card — the hardware that connects a server to the exchange network) to an order being sent back out is measured in microseconds. A single heap allocation can take anywhere from 200 nanoseconds to several microseconds depending on the allocator's state — enough to miss the trading window entirely. For this reason, production HFT systems enforce a strict rule: **no heap allocation on the hot path** (the code that executes for every incoming market event). All `new`/`delete` calls happen during a startup phase, before trading begins.

A concrete example: a market data handler (the component that receives and parses incoming price updates from the exchange) typically maintains pre-sized buffers and fixed arrays for holding recent ticks. These are allocated once at startup with `new[]`. During live trading, the handler only reads from and writes to these already-established pointers. From the hot path's perspective there is no difference between a heap array set up at startup and a stack array — both are just addresses the CPU reads and writes at full speed, with no allocator involvement.

The stack, meanwhile, shines for temporary work inside a single function. A function that computes a 20-period moving average (the average of the last 20 prices — a simple trend indicator) might declare a small local `double` array of 20 elements, fill it with recent prices, compute the sum, and return — all without touching the heap. That local array is allocated and torn down in nanoseconds with zero allocator overhead. Understanding that stack variables disappear when a function returns is equally critical: you must never return a pointer to a local variable, because the stack frame is gone the moment the caller receives the pointer. This mistake crashes real production systems.

## Pitfalls

- **Returning a pointer to a local variable.** The local is destroyed when the function returns; the caller gets a dangling pointer to freed memory. Objects that need to outlive their enclosing function must live on the heap.

- **Forgetting `delete` (memory leak).** Every `new` must have exactly one matching `delete`. If you lose the last pointer to a heap object without deleting it, that memory is unrecoverable until the process exits.

- **Using `delete` instead of `delete[]` for heap arrays.** `new int32_t[100]` must be freed with `delete[]`, not `delete`. The wrong form is undefined behavior and may silently corrupt the heap.

- **Using a pointer after `delete` (dangling pointer).** After `delete p`, the pointer still holds an address, but the memory at that address is no longer yours. Set `p = nullptr` immediately after every `delete` to convert accidental use into a predictable null-pointer crash.

- **Stack overflow from large local arrays.** `double data[1000000]` inside a function requests 8 MB of stack — likely crashing the program. Arrays larger than a few thousand elements should live on the heap.

- **Mismatching `new` with `delete[]` or vice versa.** Using `delete[]` on memory from plain `new` is equally undefined. The rule is simple: always match the allocation form.

## Exercise

Goal: allocate a heap array, process it with pointer arithmetic from Day 5, and clean up correctly.

1. Create `20260504/day06.cpp`. Include `<iostream>` and `<cstdint>`. Write `main()` returning `0`.

2. Inside `main`, allocate a heap array of 6 `int32_t` values:
   ```cpp
   int32_t* ticks = new int32_t[6];
   ```
   Fill it with `{210, 208, 211, 209, 212, 207}` using a `for` loop with an index variable `i` from `0` to `5`.

3. Find and print the maximum value using a pointer loop (not index-based): declare `int32_t* end = ticks + 6;`, a variable `int32_t max_val = *ticks;`, and a pointer `cur` starting at `ticks`. Loop `while (cur != end)` — if `*cur > max_val`, update `max_val`; then `++cur`. Print `max_val`. Expected: `212`.

4. Compute and print the sum of all six values with the same pointer-loop pattern. Expected sum: `1257`.

5. Free the array and null the pointer, then print a confirmation:
   ```cpp
   delete[] ticks;
   ticks = nullptr;
   std::cout << "Memory released.\n";
   ```

   Compile:
   ```bash
   g++ -std=c++17 -Wall -Wextra day06.cpp -o day06
   ./day06
   ```

Self-check: the program prints maximum `212`, sum `1257`, and `Memory released.` with no compiler warnings.

## Glossary

- **Automatic storage duration** — the lifetime rule for local variables: memory is allocated when the enclosing scope is entered and freed when it is exited, automatically.
- **Stack overflow** — a runtime crash that occurs when a program tries to use more stack space than the OS permits (typically 8 MB on Linux).
- **Memory leak** — heap memory that was allocated with `new` but never released with `delete`, making it permanently unavailable for the rest of the program's run.
- **Dangling pointer** — a pointer that holds an address of memory that has already been freed. Using it is undefined behavior.
- **Hot path** — the sequence of code executed for every time-critical event (e.g., each incoming market data message). In HFT, this code must avoid any operation with unpredictable latency. Covered in depth starting Day 159.
- **NIC** — network interface card; the hardware component that connects a server to the exchange's network.

## Tomorrow's Preview

**Day 7 — Functions: pass by value, ref, const ref.** Now that you know where variables live, tomorrow covers what happens to memory when you pass a variable into a function — whether a fresh copy is made on the stack, or the original is shared via a reference.

## Links

- [Roadmap](../cpp-roadmap.md)
- Previous: [Day 5 — Pointer Arithmetic](2026-05-03-day-05-pointer-arithmetic.md)
