---
date: 2026-04-29
day: 1
phase: 01-foundations
tags: [cpp, quant, compilation, toolchain]
---

# Day 1 — How a C++ File Becomes a Program

> **Why this matters:** Welcome to Day 1. Before writing any C++, you need a clear mental picture of what happens when you "build" a program. C++ is unlike Python or JavaScript — it doesn't run from source. It gets translated into raw machine instructions ahead of time. Understanding that translation process makes most error messages you'll see in the next few weeks suddenly make sense.

## Concept

When you run a Python file, the Python interpreter reads it line by line and executes it. C++ doesn't work that way. A C++ source file (`.cpp`) has to be turned into a real program (an *executable*) before it can run. That translation is called **compilation**, and it happens in stages.

Think of it like this analogy. You wrote a recipe in English. Before a French chef who speaks no English can cook it, someone has to: (1) replace any "see appendix" notes with the actual appendix text, (2) translate the recipe into French, (3) bind it together with any sub-recipes it references. Only then can the chef cook. C++ compilation is the same idea.

There are four stages.

**Stage 1 — Preprocessor.** This stage handles every line in your file that starts with `#`. The most important one is `#include <iostream>`. The preprocessor literally finds the `iostream` file on your system and pastes its entire contents into your file at that spot. After this stage, your "file" is much bigger than what you typed — often thousands of lines longer — because it now contains all the included content. The result is called a **translation unit** (a fancy name for "one source file with all its includes pasted in").

**Stage 2 — Compiler.** Now the compiler takes that big translation unit and translates it into something close to machine code, called an **object file** (it ends in `.o` on Linux/Mac). This is where most syntax errors come from — missing semicolons, undeclared variables, type mismatches. Important fact: each `.cpp` file is compiled *on its own*, in isolation. Two `.cpp` files don't see each other at this stage.

**Stage 3 — Assembler.** Turns the compiler's output into actual machine instructions. You almost never see this stage — it's bundled inside the compiler step. Mention it for completeness; you can ignore it for now.

**Stage 4 — Linker.** Now you have one or more `.o` files. The linker stitches them together into a single executable. If `main.cpp` calls a function named `add` that lives in `math.cpp`, the linker is the one that connects them. Errors here look like `undefined reference to 'add'` — meaning "you promised this function existed somewhere, but I can't find it."

> **Key idea:** Header files (`.h`) make *promises* about functions and types. Source files (`.cpp`) *deliver* on those promises. The compiler reads promises; the linker checks deliveries.

> **Note:** Compiling and linking separately means if you change one `.cpp` file, only that file needs to be recompiled — not your whole project. On a small project this saves seconds; on a real trading system with thousands of files, it saves hours.

## Code Example

Let's see all four stages with a tiny program. Create three files in the same folder.

`greet.h` — a *promise* that a function called `greet` exists:
```cpp
// greet.h
// This is a header file. It declares (promises) a function.

void greet();  // declaration: "there is a function called greet that takes nothing and returns nothing"
```

`greet.cpp` — *delivers* on that promise:
```cpp
// greet.cpp
#include <iostream>   // pull in std::cout
#include "greet.h"    // pull in our own promise so the compiler can check we match it

void greet() {        // definition: the actual code
    std::cout << "Hello from a C++ program\n";
}
```

`main.cpp` — uses `greet`:
```cpp
// main.cpp
#include "greet.h"    // see the promise (so we know greet() exists)

int main() {
    greet();          // call it
    return 0;
}
```

Now build it. Open a terminal in the folder containing those three files and run:

```bash
g++ -std=c++17 -Wall greet.cpp main.cpp -o hello
```

What that command means: `g++` is the compiler. `-std=c++17` says use C++17 rules. `-Wall` turns on helpful warnings. The two `.cpp` files are inputs. `-o hello` names the output executable `hello`.

Run it:
```bash
./hello
```

Expected output:
```
Hello from a C++ program
```

Behind the scenes, `g++` ran all four stages for both files and linked the results. You can also run the stages separately:

```bash
g++ -std=c++17 -c greet.cpp    # produces greet.o (compile only, no link)
g++ -std=c++17 -c main.cpp     # produces main.o
g++ greet.o main.o -o hello    # link the .o files into an executable
```

Same outcome. The split shows you the seams.

## Quant Application

A *trading system* is a program that watches market prices and decides when to buy or sell. Real ones live in C++ because every microsecond of delay is a worse price. Even at the entry-level scale, a trading system is split into many small `.cpp` files: one for reading **market data** (the live stream of price updates from an exchange), one for the trading logic, one for risk checks, one for talking to the exchange, and so on. Understanding the compilation model matters from day one because trading codebases are *organized* around it.

Here's the practical impact. Suppose a developer changes one line in the risk-check file. If the build system understands the compilation model, only that one file recompiles, plus the link step — maybe five seconds. If they didn't, the whole 500-file project rebuilds — five minutes. Now multiply by 50 builds a day across 30 developers. The compilation model is, in a real economic sense, money. This is why trading firms invest heavily in build hygiene: small, focused header files; minimal includes; clear separation between promises (`.h`) and deliveries (`.cpp`).

Second, the linker errors you'll spend the next few weeks bumping into ("undefined reference," "multiple definition") are the same errors a quant developer hits on a 1-million-line codebase — just bigger. Building the mental model now, on a 3-file toy project, is exactly the practice you want. When something breaks at your future job, you'll think "the linker can't find a definition" instead of panicking. That's the move from beginner to confident developer.

## Pitfalls

- **Forgetting the `;` after a class or struct declaration in a header.** The compiler error often points to the line *after* the missing semicolon, which is confusing. If an error mentions a strange line, look just above it.
- **Putting `using namespace std;` in a header file.** It leaks into every file that includes it. For now: don't write `using namespace std;` at all — type `std::cout` and `std::string` explicitly. It's a few extra characters and it'll save you headaches.
- **Calling `g++ main.cpp` and forgetting `greet.cpp`.** You'll get `undefined reference to greet()` — the linker is telling you nobody delivered on the promise. Fix: include all your `.cpp` files in the build command.
- **`#include`ing a `.cpp` file.** Don't. Always include `.h` headers, never `.cpp` source files. Including a `.cpp` makes its definitions appear twice (once on its own, once where you included it) and the linker will complain.
- **Saving the file but forgetting to re-run `g++`.** The executable doesn't update by itself. Edit, rebuild, run — every time.

## Exercise

Goal: feel each stage of the build by deliberately breaking it.

1. Create the three files from the Code Example above (`greet.h`, `greet.cpp`, `main.cpp`) in a fresh folder. Run `g++ -std=c++17 -Wall greet.cpp main.cpp -o hello` and then `./hello`. Confirm you see `Hello from a C++ program`.

2. **Break the linker.** Run `g++ -std=c++17 -Wall main.cpp -o hello` (omit `greet.cpp`). Read the error carefully. You should see `undefined reference to 'greet()'`. That's the linker — stage 4 — saying "the promise was made but never delivered." Restore the working build.

3. **Break the compiler.** Open `main.cpp` and change `greet();` to `greet(;` (delete the closing paren). Rebuild. The error will mention an unexpected token or expected `)`. That's the compiler — stage 2 — saying your syntax is wrong. Fix it.

4. **Break the preprocessor.** Open `main.cpp` and change `#include "greet.h"` to `#include "greeeet.h"` (typo). Rebuild. You should see `fatal error: greeeet.h: No such file or directory`. That's stage 1 — the preprocessor can't find a file to paste in. Fix it.

You should be able to look at any error message and say "that's the preprocessor / compiler / linker complaining."

## Glossary

These terms appeared with brief asides above; the day they're covered in depth is in parentheses. Don't worry about memorizing — they'll come around again.

- **Translation unit** — one `.cpp` file plus everything its `#include`s pulled in. Covered properly on Day 11.
- **Market data** — the live stream of price updates from an exchange. Used throughout Phase 7.
- **Risk check** — code that prevents the system from sending an order that loses too much money or breaks a rule. Phase 7.

## Tomorrow's Preview

**Day 2 — Fundamental types and sizes.** `int`, `double`, `char`, and friends. You'll see why C++ has so many number types where Python has just one, and how to ask the compiler "how many bytes does this take?" Builds on today: each `.cpp` file is compiled independently, and types are the alphabet the compiler reads.

## Links

- [Roadmap](../cpp-roadmap.md)
