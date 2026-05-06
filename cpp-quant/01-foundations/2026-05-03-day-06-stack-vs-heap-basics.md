---
date: 2026-05-03
day: 6
phase: 01-foundations
tags: [cpp, quant, memory, stack, heap]
---

# Day 6 — Stack vs Heap Basics

> **Why this matters:** Every variable in your program lives somewhere in memory — but not all memory is the same. C++ gives you two fundamentally different memory regions: the stack and the heap. Understanding which one holds your data determines how long it lives, how fast it is to allocate, and how much you can have at once. This distinction underpins nearly every design decision in performance-critical C++ code, from simple functions to full trading systems.

## Concept

### Two regions of memory

When a C++ program runs, the operating system hands it several regions of memory. Two of them you will interact with constantly:

- The **stack** — a compact, fixed-size region where local variables live automatically.
- The **heap** (also called the **free store**) — a much larger region where you can request memory explicitly, whenever you need it and for however long you need it.

Every variable you have written so far — `int32_t x = 5;`, `double price = 100.25;`, `int32_t* p = &x;` — has been a stack variable. You declared it, used it, and when the surrounding block of code ended, it was gone. You did not have to do anything to clean it up. That automatic behavior is the defining characteristic of the stack.

### The stack: automatic lifetime

Picture the stack as a stack of trays in a cafeteria. Each function call (including `main`) gets its own tray — a block of memory called a **stack frame** — that holds all the local variables for that call. When the function returns, its tray is removed from the top and all its variables vanish. The next function call gets a fresh tray placed on top.

The operating system reserves a fixed amount of memory for the stack at program startup — typically 1 MB to 8 MB on modern Linux and Windows systems. The CPU manages it with a special register called the **stack pointer**, which records where the current top of the stack is. Allocating a local variable is as fast as adjusting that one register by the size of the variable. There is nothing to search, no bookkeeping structure to update. Allocation and deallocation are essentially free in terms of CPU time.

There is a catch: the stack has limited capacity. Declare too many large local variables and you will hit a **stack overflow** — the program crashes because it tried to expand the stack past the OS limit. For small variables like `int32_t` and `double` this is never a problem. For large data — an array of a million integers, a big lookup table — the stack is the wrong place.

> **Key idea:** Stack variables are created instantly when their scope begins and destroyed automatically when their scope ends. You pay almost no runtime cost for this — the CPU's function-call mechanism handles it for you.

### Scope controls stack lifetime

You do not have to be in a separate function to see this in action. In C++, braces `{}` create a new **scope**. Any variable declared inside a pair of braces is destroyed when execution reaches the matching closing brace:

```cpp
int32_t x = 10;    // x is on the stack; lives until main() exits
{
    int32_t y = 20;   // y is on the stack; lives only until the closing brace
    // Both x and y are usable here
}
// y is gone here — the compiler will reject any attempt to use it
// x is still alive
```

This rule is called **automatic storage duration**: a variable's lifetime is tied to its enclosing scope, with no cleanup code you have to write.

### The heap: dynamic lifetime

Sometimes you need memory that:
- Outlives the function that created it, or
- Is too large to fit on the stack, or
- Has a size that is not known until the program is actually running.

For those cases, you use the heap. The heap is a much larger pool of memory — on a modern machine it is limited by available RAM, often gigabytes. Unlike the stack, heap memory does not have an automatic lifetime. You request it explicitly with `new`, use it, and release it explicitly with `delete`.

```cpp
int32_t* p = new int32_t(42);   // ask the heap for one int32_t, initialized to 42
                                 // p (the pointer variable itself) lives on the stack
*p = 100;                        // modify the heap-allocated int32_t
delete p;                        // return the memory to the heap
p = nullptr;                     // good habit: mark the pointer as "points to nothing"
```

(`nullptr` is C++'s keyword for "this pointer points to nothing valid" — think of it as address zero. We will cover it formally on Day 49; for now just know that setting a pointer to `nullptr` after `delete` is standard defensive practice.)

`new` asks the allocator for enough space to hold one `int32_t`, stores the value 42 there, and returns the address as a pointer. That pointer is a normal stack variable — 8 bytes on a 64-bit machine. The `int32_t` it *points to* lives on the heap.

`delete` returns that heap block to the allocator so it can be reused. After `delete`, the pointer still holds the old address, but that address no longer belongs to you. Dereferencing it is undefined behavior — the program can crash, silently corrupt data, or appear to work correctly. Setting `p = nullptr` immediately after `delete` turns any accidental later use of `p` into a detectable null-pointer dereference rather than silent corruption.

> **Key idea:** Heap memory lives until you explicitly release it with `delete`, regardless of what function or scope created it. This power comes with responsibility: forget to call `delete`, and that memory is locked away (a **memory leak**) until the program exits.

We will cover `new` and `delete` in much greater depth on Day 37, including why modern C++ gives you smarter alternatives. For today, the goal is to understand *what* the heap is and *why* it exists.

### Stack vs heap at a glance

| Property | Stack | Heap |
|---|---|---|
| Allocation cost | Nearly free (adjust one register) | Slower (allocator searches for a free block) |
| Deallocation | Automatic (scope exit) | Manual (`delete`) |
| Capacity | Small (1–8 MB typical) | Large (limited by RAM) |
| Lifetime | Tied to enclosing scope | Until `delete` is called |
| Main risk | Stack overflow if too large | Memory leak if `delete` is forgotten |

### How the heap allocator works (simplified)

When you call `new`, the C++ runtime delegates to a component called the **allocator**. The allocator maintains an internal record of which blocks of heap memory are free and which are in use. Finding a free block of the right size requires consulting that record — considerably more work than adjusting a single register. This is why heap allocation is measurably slower than stack allocation, and why latency-sensitive code (such as an HFT system where a microsecond matters) avoids calling `new` in hot paths. We will quantify this cost with benchmarks on Day 128; for now, the important intuition is that the stack is always faster.

## Code Example

Create `stack_heap.cpp`:

```cpp
// stack_heap.cpp — observing stack vs heap lifetime and addresses

#include <iostream>
#include <cstdint>

int main() {
    // --- Stack variables ---
    int32_t a = 10;
    int32_t b = 20;

    // Print value and address of each stack variable
    std::cout << "Stack a: value=" << a << "  address=" << &a << "\n";
    std::cout << "Stack b: value=" << b << "  address=" << &b << "\n";
    // a and b are adjacent on the stack; addresses should differ by exactly 4 bytes

    // --- Scope demonstration ---
    {
        int32_t c = 30;   // c lives only inside these braces
        std::cout << "Inner scope c: value=" << c << "  address=" << &c << "\n";
    }
    // c no longer exists here; the compiler would reject: std::cout << c;

    // --- Heap allocation ---
    // The pointer hp lives on the stack.
    // The int32_t that hp points to lives on the heap.
    int32_t* hp = new int32_t(99);

    std::cout << "\nHeap value:              " << *hp << "\n";
    std::cout << "Heap address (hp):       " << hp  << "\n";
    std::cout << "Pointer hp on stack (&hp): " << &hp << "\n";
    // Notice: hp (heap address) is in a very different range from &hp (stack address)

    // --- Heap memory persists across scopes ---
    {
        // We can reach the heap memory from any scope, as long as we hold the pointer
        *hp = 777;
        std::cout << "Modified inside inner scope: " << *hp << "\n";
    }
    // The heap memory is still alive here; the inner scope didn't destroy it
    std::cout << "Still accessible after inner scope: " << *hp << "\n";

    // --- Release the heap memory ---
    delete hp;
    hp = nullptr;   // hp is now a null pointer; accidental use will crash predictably

    std::cout << "Heap memory released.\n";
    return 0;
}
```

Compile and run:

```bash
g++ -std=c++17 -Wall -Wextra stack_heap.cpp -o stack_heap
./stack_heap
```

Expected output (exact addresses will differ on every run — address-space layout randomization means the OS picks different base addresses each time):

```
Stack a: value=10  address=0x7ffd1a2b3c10
Stack b: value=20  address=0x7ffd1a2b3c0c
Inner scope c: value=30  address=0x7ffd1a2b3c08

Heap value:              99
Heap address (hp):       0x5647e8f2a2b0
Pointer hp on stack (&hp): 0x7ffd1a2b3c00
Modified inside inner scope: 777
Still accessible after inner scope: 777
Heap memory released.
```

Notice three things in the output. First, `a` and `b` have addresses 4 bytes apart — they are sitting right next to each other on the stack frame (4 bytes = `sizeof(int32_t)`). Second, the heap address stored in `hp` (`0x5647...`) is in a completely different numeric range from the stack addresses (`0x7ffd...`). Third, the modification made inside the inner scope persists after the brace closes — the heap memory outlives the scope, unlike `c` which was gone the moment its `}` was reached.

## Quant Application

In a trading system, **market data** (the stream of price and trade events broadcast by an exchange — every bid, ask, and execution; a **bid** is the highest price a buyer is willing to pay; an **ask** is the lowest price a seller will accept) flows in continuously during market hours. The receive buffer — the block of memory the OS kernel writes incoming network bytes into — almost always lives on the heap. It needs to hold potentially megabytes of burst data, and it must survive across many function calls as different layers of the system process each message. Stack buffers are limited to a few kilobytes and vanish the moment the allocating function returns. The choice is forced: heap for large, long-lived buffers; stack for small, short-lived temporaries.

The **order book** — the complete list of all resting buy and sell orders for a security organized by price level — is the canonical example of a data structure that must live on the heap. An active order book for a busy stock might hold thousands of outstanding orders, each with a price, quantity, and timestamp, and it must survive for the entire trading session, growing and shrinking continuously as orders arrive and are matched. Stack memory could never hold it (too large, wrong lifetime). The standard approach is to allocate the order book's internal storage on the heap once at startup, hold it via a pointer for the whole session, and free it only when the program shuts down. When we build a matching engine starting on Day 134, this is exactly the allocation pattern you will write.

High-frequency trading (HFT) firms — trading operations that execute orders in microseconds or nanoseconds, where a one-microsecond delay can cost real money — take heap management one step further: they **pre-allocate** all the heap memory they will ever need before the market opens. The allocator's bookkeeping work happens once, during a startup phase when latency does not matter. During the trading session, the hot path (the sequence of code that runs for every incoming tick) reuses pre-allocated slots and never calls `new` or `delete`. Stack allocation — which requires no allocator call, just a register adjustment — remains the go-to choice for small temporaries in that same hot path. Understanding the cost gap between stack and heap is the first step toward writing latency-conscious C++.

## Pitfalls

- **Forgetting `delete`.** Every `new` must have exactly one matching `delete`. Skip the `delete` and you have a memory leak: that heap block is reserved for your process until the program exits. In a long-running trading system, leaks accumulate silently and eventually exhaust available memory.

- **Using memory after `delete` (use-after-free).** Once `delete p` is called, the memory `p` pointed to no longer belongs to you. Reading or writing `*p` afterward is undefined behavior — the program may crash immediately, silently corrupt other data, or appear to work. Always set `p = nullptr` immediately after `delete` to catch accidental reuse early.

- **Calling `delete` twice on the same pointer.** Calling `delete` on a pointer twice is undefined behavior. Setting `p = nullptr` right after the first `delete` prevents this: `delete nullptr` is explicitly defined by C++ to do nothing, so a second `delete` on a nulled pointer is harmless.

- **Stack overflow from large local arrays.** Declaring a very large local variable — for example `int32_t buf[500000];` — places roughly 2 MB on the stack, which exceeds the typical stack limit. The program crashes, often with a cryptic "segmentation fault" rather than a clear error message. Large buffers belong on the heap.

- **Returning a pointer to a local variable (dangling pointer).** Once a function returns, all its local (stack) variables are destroyed. A pointer to one of them is immediately dangling. You have not written multi-function programs yet, but keep this rule in mind for Day 7: never return the address of a local variable.

- **Confusing the pointer with what it points to.** The pointer `p` is a small stack variable (8 bytes on a 64-bit machine). It holds an address. After `delete p`, the pointer variable `p` still exists on the stack — it just holds an address that no longer belongs to you. The pointer is not gone; the *memory it pointed to* is gone. This is why nulling the pointer (`p = nullptr`) is necessary: the pointer is still alive and could be accidentally used.

## Exercise

Goal: observe stack lifetime and heap lifetime hands-on by printing addresses and watching when values disappear.

1. Create `20260503/day06.cpp`. Include `<iostream>` and `<cstdint>`. Write `main()` returning 0. Declare three `int32_t` stack variables: `x = 1`, `y = 2`, `z = 3`. Print each value and each address (using `&x`, `&y`, `&z`). Run the program and confirm the addresses are close together.

2. Add a nested scope inside `main`:
   ```cpp
   {
       int32_t inner = 99;
       std::cout << "inner value: " << inner << "  address: " << &inner << "\n";
   }
   // inner is gone here
   ```
   After the closing `}`, add the comment shown. Confirm that the compiler rejects the program if you add `std::cout << inner;` after the brace, then remove that line before moving on.

3. After the nested scope, allocate one `int32_t` on the heap:
   ```cpp
   int32_t* hp = new int32_t(42);
   ```
   Print `*hp` (the value) and `hp` (the heap address). Also print `&hp` (the address of the pointer variable itself, which is on the stack). Confirm that `hp` (heap address) is far from `&hp` (stack address).

4. Inside another nested scope `{ ... }`, change the heap value: `*hp = 1000;`. Close the scope. Then print `*hp` outside the scope to confirm it is still 1000 — the heap memory survived the scope exit.

5. Release the memory: `delete hp; hp = nullptr;`. Print `"Released.\n"`.

   Compile and run:
   ```bash
   g++ -std=c++17 -Wall -Wextra day06.cpp -o day06
   ./day06
   ```

   Self-check: `x`, `y`, `z` addresses are within a few bytes of each other; the heap address in `hp` is in a very different numeric range from `&hp`; `*hp` is 1000 after the inner scope; the program prints "Released." and exits cleanly without crashing.

## Glossary

- **Stack frame** — the block of memory reserved for one function call; holds all that function's local variables. Seen more clearly on Day 7 when we write our own functions.
- **Stack overflow** — a crash caused by exhausting the fixed stack memory region, typically by declaring overly large local variables or deeply nesting function calls.
- **Memory leak** — heap memory allocated with `new` that is never returned with `delete`; it remains reserved until the program exits.
- **Dangling pointer** — a pointer that holds the address of memory that has already been freed or gone out of scope; dereferencing it is undefined behavior.
- **nullptr** — C++'s keyword for a null pointer (a pointer that points to nothing). Setting a pointer to `nullptr` marks it as invalid. Covered formally on Day 49.
- **new / delete** — the C++ operators for allocating and freeing heap memory. Covered in depth on Day 37, including modern alternatives.
- **Smart pointer** — a wrapper type that automatically calls `delete` when it goes out of scope, eliminating manual memory management. Covered on Days 38–40.
- **Order book** — the complete list of all resting buy and sell orders for a security at each price level. Covered starting on Day 134.
- **HFT (High-Frequency Trading)** — trading strategies executed in microseconds or nanoseconds, where even a single unexpected heap allocation can meaningfully impact latency.

## Tomorrow's Preview

**Day 7 — Functions: Pass by Value, Ref, Const Ref.** Now that you understand what the stack is and that each function call gets its own frame, you will see exactly what happens to a variable when it is passed into a function — and why passing by reference can avoid an unnecessary copy of the entire variable.

## Links

- [Roadmap](../cpp-roadmap.md)
- Previous: [Day 5 — Pointer Arithmetic](2026-05-03-day-05-pointer-arithmetic.md)
