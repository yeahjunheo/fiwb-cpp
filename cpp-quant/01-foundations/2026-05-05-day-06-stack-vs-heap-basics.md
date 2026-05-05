---
date: 2026-05-05
day: 6
phase: 01-foundations
tags: [cpp, quant, memory, stack, heap, dynamic-allocation]
---

# Day 6 — Stack vs Heap Basics

> **Why this matters:** Every variable you declare lives somewhere in memory. So far, all your variables have been local — declared inside a function, disappearing when the function ends. Today you learn the two main memory regions a C++ program uses — the **stack** and the **heap** — what makes them different in speed and lifetime, and how to choose between them. This distinction comes up in virtually every C++ design decision, from simple utility code to high-performance trading systems.

## Concept

### Two regions, two rules

When a C++ program runs, the operating system gives it a block of virtual memory. Two named regions inside that block serve different purposes:

- **The stack** — a fixed-size region (typically 1–8 MB on Linux and Windows by default) for local variables and function call bookkeeping. The runtime manages it automatically.
- **The heap** (also called the *free store*) — a large region, limited mainly by available RAM, for memory you explicitly request during program execution. You manage it manually with `new` and `delete`.

The key difference is **who is responsible** for each piece of memory:
- Stack memory: the compiler and runtime handle creation and destruction automatically.
- Heap memory: your code is responsible. You request it, and you must release it.

### The stack: automatic storage

Picture a stack of plates. Each time you enter a new scope — a function body, an `if` block, a loop body — the program pushes a "plate" called a **stack frame** onto the stack. The frame holds all local variables declared in that scope. When the scope ends, the frame is popped off, and those variables are destroyed automatically.

This "last in, first out" order matches how the hardware works. Entering a scope moves the **stack pointer** (a CPU register that tracks the top of the used stack space) by the total size of the local variables. Leaving a scope moves it back by the same amount. Allocation and deallocation are effectively a single register instruction each — nearly free in CPU terms.

Variables on the stack have **automatic storage duration**: their lifetime is tied to the enclosing scope. When the scope ends, they are gone.

```cpp
void foo() {
    int32_t x = 10;     // x lives on the stack; created here
    {
        int32_t y = 20; // y lives on the stack; created here
    }                   // y is destroyed here — scope ends
    // x still exists; y does not
}                       // x is destroyed here
```

The stack's size is fixed when the program starts. If your code creates so many nested stack frames that the pointer runs out of room — most commonly from deep or infinite recursion — the program crashes with a **stack overflow**. The name is literal: the pointer has overflowed past the allocated stack space.

### The heap: dynamic storage

Sometimes you do not know at compile time how large something needs to be, or you need memory to outlive the function that created it. That is when you use the heap.

You request heap memory with the **`new` expression**. `new` does two things: it asks the operating system for a block of memory large enough for the requested type, and it returns a pointer to that block.

```cpp
int32_t* p = new int32_t(42);  // allocate one int32_t on the heap, initialize to 42
                                // p itself lives on the stack; the int32_t lives on the heap
```

Notice the separation: the pointer `p` is a local variable — it lives on the stack. The `int32_t` that `p` points to lives on the heap.

> **Key idea:** `new` allocates memory on the heap and returns a pointer to it. The pointer variable itself still lives on the stack.

When you are done with heap memory, you must release it with **`delete`**:

```cpp
delete p;      // free the heap memory that p points to
p = nullptr;   // best practice: null the pointer so it cannot be accidentally used
```

`nullptr` is a special pointer value meaning "points to nothing." Setting a pointer to `nullptr` after deleting it makes any accidental later use immediately obvious — you get a clean crash rather than silent data corruption from reading a garbage address.

> **Key idea:** Every `new` must be paired with exactly one `delete`. Forgetting `delete` is called a **memory leak** — the memory stays reserved until the program exits, even though you can no longer reach it.

### Heap arrays

To allocate an array on the heap, use `new[]` and `delete[]`:

```cpp
int32_t n = 100;              // size known only at runtime
int32_t* arr = new int32_t[n]; // allocate 100 int32_ts on the heap
arr[0] = 10;                   // use it exactly like a stack array
// ... work with arr ...
delete[] arr;                  // must use delete[], not delete
arr = nullptr;
```

The `[]` on `delete` is mandatory for arrays. Using plain `delete` on a heap array is undefined behavior — the runtime does not know how many elements to clean up.

### Stack vs heap at a glance

| Property | Stack | Heap |
|---|---|---|
| Typical size | 1–8 MB | Limited by available RAM |
| Allocation speed | Very fast (one instruction) | Slower (OS bookkeeping required) |
| Variable lifetime | Tied to enclosing scope | Until you call `delete` |
| Who manages it | Runtime, automatically | You, with `new` and `delete` |
| Main risk | Stack overflow (too deep) | Memory leak (forgot `delete`) |

### When to use each

Use the stack for everything you can: local variables, small fixed-size arrays, function parameters. They are automatically cleaned up and very fast to allocate.

Use the heap when:
- The memory must outlive the function that creates it.
- The size is only known at runtime (for example, a number the user types in determines how many elements to store).
- The data is large enough to risk a stack overflow (anything over a few hundred kilobytes is risky as a local variable).

## Code Example

Create `stack_heap.cpp`:

```cpp
// stack_heap.cpp — stack vs heap allocation

#include <iostream>
#include <cstdint>

int main() {
    // --- Stack allocation ---
    int32_t price_stack = 1050;
    std::cout << "Stack price: " << price_stack << "\n";
    std::cout << "Address:     " << &price_stack << "\n";

    // --- Heap allocation: single value ---
    int32_t* price_heap = new int32_t(2050);
    std::cout << "\nHeap price:  " << *price_heap << "\n";
    std::cout << "Address:     " << price_heap << "\n";
    delete price_heap;
    price_heap = nullptr;   // null immediately after freeing

    // --- Heap allocation: array ---
    int32_t count = 5;
    int32_t* ticks = new int32_t[count];

    for (int32_t i = 0; i < count; ++i) {
        ticks[i] = 100 + i;   // 100, 101, 102, 103, 104
    }

    // Traverse with pointer arithmetic (from Day 5)
    std::cout << "\nHeap tick array: ";
    int32_t* cur = ticks;
    int32_t* end = ticks + count;
    while (cur != end) {
        std::cout << *cur << " ";
        ++cur;
    }
    std::cout << "\n";

    delete[] ticks;   // must match new[] with delete[]
    ticks = nullptr;

    // --- Stack lifetime demo ---
    {
        int32_t temp = 999;   // created here
        std::cout << "\nInside scope: temp = " << temp << "\n";
    }
    // temp does not exist here; the stack frame was unwound
    // std::cout << temp;   // would not compile — temp is out of scope

    return 0;
}
```

Compile and run:

```bash
g++ -std=c++17 -Wall -Wextra stack_heap.cpp -o stack_heap
./stack_heap
```

Expected output (your addresses will be different every run):

```
Stack price: 1050
Address:     0x7ffd... (some high address — stack lives near the top of virtual memory)

Heap price:  2050
Address:     0x55f...  (some lower address — heap lives lower in virtual memory)

Heap tick array: 100 101 102 103 104

Inside scope: temp = 999
```

Notice that the stack address typically starts with `0x7fff...` on a 64-bit Linux machine, while the heap address starts with a much lower number. The addresses change each run because of a security feature called **ASLR** (address space layout randomization), which randomizes where memory regions are placed in virtual memory — you do not need to understand this now; just observe that the two regions are in very different parts of memory.

## Quant Application

The stack-vs-heap distinction is an active design constraint in performance-critical trading code. In **HFT** (high-frequency trading — trading strategies that make decisions in microseconds), calling `new` on the "hot path" (the code executed for every incoming market message) is avoided because `new` requires the operating system to find and record a free block of memory, which can take hundreds of nanoseconds. That sounds tiny, but at HFT speeds it can mean missing a trading opportunity. Stack variables, by contrast, cost a single CPU instruction to allocate. This is why HFT systems pre-allocate all their data structures once at startup (using `new` during initialization) and then reuse those same blocks throughout the trading session, never calling `new` while actively trading.

**Market data** (the stream of price and trade updates broadcast by an exchange — think of it as a firehose of numbers saying "this stock just traded at this price") arrives at rates of thousands to millions of messages per second on a busy equity market. A market data handler typically allocates a fixed-size receive buffer on the heap once during startup, then reuses that same buffer for every incoming message, overwriting old data with new. The buffer's lifetime must outlast the setup function that creates it — it needs to exist for the entire trading session — which is exactly the use case for heap allocation. A stack-allocated buffer would be destroyed the moment the setup function returned, leaving the handler with a dangling pointer.

An **order book** (the complete ledger of active buy and sell orders for a security — it lists every price level where someone is willing to buy or sell, and how many shares they want) is another structure that lives on the heap. An order book persists for the entire trading day, is updated with every incoming order, and must be accessible from many different parts of the system. Allocating it with `new` and passing around a pointer to it is the natural design. We will build a real order book starting on Day 134; the key intuition for now is: if data needs to outlive the function that creates it, or if its size is not known at compile time, the heap is the right home for it.

## Pitfalls

- **Memory leak: forgetting `delete`.** If you allocate with `new` and never call `delete`, that memory stays reserved until the program exits. A long-running trading process that leaks on every incoming message will eventually exhaust available RAM and crash — sometimes hours or days after the leak was introduced.

- **Using `delete` instead of `delete[]` for arrays.** Memory allocated with `new T[n]` must be freed with `delete[]`, not `delete`. Using the wrong form is undefined behavior. The compiler will not warn you about this mismatch.

- **Using memory after `delete` (dangling pointer).** Once you call `delete p`, the heap block `p` points to is gone. Reading or writing through `p` afterward is undefined behavior — the memory may now belong to a completely different variable. Setting `p = nullptr` immediately after every `delete` is the simplest defense.

- **Double-delete.** Calling `delete p` twice on the same address is undefined behavior and often causes a crash. This commonly happens when two pointers both point to the same heap block and both try to free it. Again, setting to `nullptr` after delete helps: `delete nullptr` is defined to do nothing, so a second accidental delete of a nulled pointer is harmless.

- **Large local arrays causing stack overflow.** Declaring `int32_t big[1000000];` as a local variable puts 4 MB on the stack — more than the default 1–8 MB limit. The program will crash at runtime with no clear error message. Any array you suspect might be large should go on the heap.

- **Thinking a dangling pointer becomes null automatically.** When a stack variable goes out of scope, any pointer that was pointing to it is not automatically set to `nullptr`. It still holds the old address, which is now invalid. There is no automatic safety net; you must track variable lifetimes yourself.

## Exercise

Goal: practice allocating, using, and freeing heap memory, and observe the difference between stack and heap lifetimes.

1. Create `20260505/day06.cpp`. Include `<iostream>` and `<cstdint>`. Write `main()` returning `0`.

2. Allocate a single `int32_t` on the heap initialized to `500`. Print its value and its address (the pointer itself, not `&pointer`). Then `delete` it and set the pointer to `nullptr`. Print the pointer one more time to observe its null value.

3. Declare `int32_t count = 6;` on the stack. Allocate an array of `count` `int32_t`s on the heap. Fill it with the values `10, 20, 30, 40, 50, 60` using a `for` loop with `*(arr + i)` syntax (pointer arithmetic from Day 5, not `[]`). Print all six values by walking a pointer from `arr` to `arr + count`. Then `delete[]` the array and set the pointer to `nullptr`.

4. Inside a nested block `{ ... }`, declare a stack `int32_t local_val = 77;` and a pointer `int32_t* sp = &local_val;`. Print `*sp` inside the block. After the closing brace, add a comment: `// sp is now a dangling pointer — local_val was on the stack and is gone.` Do not use `sp` after the closing brace.

5. Compile and run:
   ```bash
   g++ -std=c++17 -Wall -Wextra day06.cpp -o day06
   ./day06
   ```

Self-check: step 2 prints `500` then a null/zero address; step 3 prints `10 20 30 40 50 60`; step 4 prints `77`. If `valgrind` is available on your system, run `valgrind ./day06` — it should report zero heap leaks.

## Tomorrow's Preview

**Day 7 — Functions: Pass by Value, Reference, and Const Reference.** Now that you understand where variables live in memory, you will see exactly what happens to that memory when you call a function — whether a copy is made on the stack, or whether the callee reaches directly into the caller's memory through a reference.

## Links

- [Roadmap](../cpp-roadmap.md)
- Previous: [Day 5 — Pointer Arithmetic](2026-05-03-day-05-pointer-arithmetic.md)
