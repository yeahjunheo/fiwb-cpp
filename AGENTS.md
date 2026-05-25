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

Do **not** spawn the editor yourself — the user edits in `nvim` in their own terminal, and launching it from Bash would either hijack the agent's terminal or detach from the user's session. After scaffolding, print the exact command for the user to copy-paste:

```
nvim exercises/<today_YYYYMMDD>/<problem-id>.cpp
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

## Toolchain: `oj` (online-judge-tools)

`oj` is installed and authenticated against AtCoder. Use it for sample testing; **never submit on the user's behalf**.

### Setup state — don't redo

- `oj` on PATH (pyenv-managed Python 3.12). Version: `online-judge-tools 11.5.1`.
- Auth via cookie at `~/Library/Application Support/online-judge-tools/cookie.jar`. The cookie is exported from a Helium browser session because Cloudflare blocks `oj login`'s direct POST and `oj`'s Selenium driver can't be pointed at Helium.
- **When the cookie expires** (~6 months from last refresh): user opens Helium → atcoder.jp → DevTools → Application → Cookies → copies the `REVEL_SESSION` value → pastes to the agent → agent splices it into the LWP-format `cookie.jar` (replace the existing `Set-Cookie3: REVEL_SESSION="..."` line).

### Testing samples

Once samples are downloaded for a problem, prefer `oj t` to manual `echo "..." | ./bin`:

```bash
cd exercises/<YYYYMMDD>
oj d <problem-url>          # one-time per problem; drops samples into ./test/
g++ -std=c++20 -Wall -Wextra <file>.cpp -o <bin>
oj t -c ./<bin>             # runs all samples, color-diffs got vs. expected
```

The `test/` subfolder is per-problem. Don't reuse one across problems. Safe to leave uncommitted or commit alongside binaries — matches the repo's existing convention.

If `oj d` hasn't been run yet for a given problem, do that first (it's a single command; samples persist). The CP problem workflow's WebFetch step still captures the problem **text** (title, restatement, constraints, format) for the agent's own understanding and the brief acknowledgement to the user; `oj d` is just for the sample I/O files on disk.

### Submission

**The user submits, not the agent.** When they're ready, they run:

```bash
oj s <problem-url> <file>.cpp
```

Don't volunteer to submit, don't ask "should I submit?", don't chain submission onto any other command.

## See also

- `CLAUDE.md` — repo structure, curriculum tracks, lesson-generation workflow, when to use web tools.
- `cp/cp-roadmap.md` — competitive-programming track roadmap (source of truth for daily progress).
- `cpp-quant/cpp-roadmap.md` — C++ language track roadmap.
