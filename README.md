# fiwb-cpp

C++ practice repo for a 168-day quant developer curriculum. Two parallel tracks run daily via scheduled tasks:

| Track | Directory | Focus | Daily Time |
|-------|-----------|-------|------------|
| **C++ Language** | `cpp-quant/` | Foundations through quant-specific C++ (RAII, templates, concurrency, lock-free structures, matching engines) | ~30 min |
| **Competitive Programming** | `cp/` | LeetCode-style problems from arrays/hashing through segment trees and bit manipulation | ~45 min |

## Structure

```
cpp-quant/           # C++ deep-dive lessons
  cpp-roadmap.md     #   curriculum tracker (168 days, 7 phases)
  01-foundations/    
  02-oop-memory/    
  03-modern-cpp/    
  04-templates-stl/ 
  05-concurrency/   
  06-performance/   
  07-quant-specific/

cp/                  # Competitive programming lessons
  cp-roadmap.md      #   curriculum tracker (168 days, 7 phases)
  01-foundations/    
  02-linear-ds/     
  03-search/        
  04-recursion-trees/
  05-dp-greedy/     
  06-graphs/        
  07-specialized-quant/

YYYYMMDD/            # Daily scratch code (compiled exercises, experiments)
```

## How it works

A scheduled Claude Code task runs daily at 4 AM. For each track it:

1. Reads `current_day` from the roadmap file
2. Generates that day's lesson as a new Markdown note in the matching phase folder
3. Marks the day complete and increments `current_day`

Progress is tracked in [`cpp-quant/cpp-roadmap.md`](cpp-quant/cpp-roadmap.md) and [`cp/cp-roadmap.md`](cp/cp-roadmap.md).