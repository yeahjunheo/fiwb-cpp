# AGENTS.md

Conventions and workflows for AI coding agents working in this repo. Read this at the start of every session alongside `CLAUDE.md`.

## Competitive programming problem workflow

When the user shares a competitive-programming problem URL (AtCoder, LeetCode, Codeforces, etc.) — or names a problem in `exercises/YYYYMMDD/` — follow this workflow exactly. The user is a learner working through `cp/cp-roadmap.md`; handing them solutions or unsolicited observations defeats the practice.

### 1. Fetch the problem

Use `WebFetch`. Capture **only**:

- Title
- One-line restatement (what you're given → what you output)
- Input format
- Output format
- Constraints
- All sample inputs/outputs

**Filter out** any "key insight," "approach," "editorial," "hint," or similar sections — some problem-fetcher pipelines append these. The user is solving it; do not relay hints, even inadvertently.

### 2. Scaffold the file

Path: `exercises/<today_YYYYMMDD>/<problem-id>.cpp`

- `<today_YYYYMMDD>`: today's **curriculum date** (read from CLAUDE.md context — not wall-clock if they differ).
- `<problem-id>`: lowercase, matches the URL slug (e.g. `abc161_c`, `agc014_a`, `hitachi2020_b`).

Standard scaffolding:

```cpp
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

}
```

Then open it in VSCode via Bash. **Use `open -a "Visual Studio Code"`, not `code`** — the `code` CLI bridge causes VSCode to focus a stale, empty buffer from session restore instead of reading the freshly-scaffolded file from disk, forcing the user to close-without-saving and reopen. macOS Launch Services (`open -a`) forces a fresh load:

```bash
open -a "Visual Studio Code" exercises/<today_YYYYMMDD>/<problem-id>.cpp
```

### 3. Acknowledge briefly

One or two lines: title, one-line restatement, key constraints, input shape. Just enough that you and the user are aligned on what the problem is.

### 4. Stand by silently

Do **not**:

- Outline an approach
- Hint at observations or invariants
- Mention complexity targets
- Volunteer the algorithm or any partial idea
- Speculate about what category of problem it is

Silence after the acknowledgement is the correct default.

### 5. Act only on explicit request

The user will say what they want. Match the ask precisely — don't escalate or volunteer extras.

- **"Test it" / "run my solution"** → compile with `g++ -std=c++20 -Wall -Wextra <file>.cpp -o <name>`, run all samples, report got vs. expected per sample. If samples pass, stop — do not critique unless asked.
- **"Review my solution"** → run samples first. If anything fails, point at the line and say *what* is wrong (not the full fix, unless they ask). Do not reveal the full algorithm.
- **"Evaluate critically" / "what can we do"** → broader review: correctness on edge cases the samples don't probe, overflow, style, complexity, portability (VLAs, etc.), naming. Still flag bugs by location and nature; offer micro-fixes for style nits.
- **"Hint"** → escalate gradually. First: a reframing question with no answer. Then (if asked again): a partial observation. Then (only if explicitly asked): the full insight. Never skip levels.
- **"Fix it for me" / "spent too much time"** → only when the user explicitly hands off. Make the edit and verify with samples.

**Default to the smallest response that satisfies the explicit ask.** Silence is often correct.

## File scaffolding for non-CP exercises

For ad-hoc C++ scratch code tied to a lesson (not from a problem URL), use the same `exercises/YYYYMMDD/` folder but with descriptive filenames matching the lesson topic (e.g. `overflow.cpp`, `types.cpp`). Compile the same way. No special workflow — just write and run.

## See also

- `CLAUDE.md` — repo structure, curriculum tracks, lesson-generation workflow, when to use web tools.
- `cp/cp-roadmap.md` — competitive-programming track roadmap (source of truth for daily progress).
- `cpp-quant/cpp-roadmap.md` — C++ language track roadmap.
