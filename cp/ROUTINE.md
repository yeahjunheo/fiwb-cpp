# CP / LeetCode Daily Practice — Claude Code Routine

Routine-ready prompt for daily LeetCode practice in C++. Paste into a new routine at https://claude.ai/code/routines.

## Setup steps

1. Go to https://claude.ai/code/routines and click **New routine**.
2. Name it: `cp-daily-problem`
3. Connect repo: `fiwb-cpp` (same repo as the C++ lessons).
4. Schedule trigger: cron `0 4 * * *`. Verify timezone matches your local zone.
5. Paste the **Routine Prompt** below into the prompt field.
6. Save and enable.
7. Optional: hit **Run now** once to verify Day 1 (Two Sum) generates cleanly and pushes to `main`.

This runs alongside the `cpp-quant-daily-lesson` routine — both fire at 4am, generating two separate commits to `main`.

## Routine Prompt

```
You are running a daily competitive programming practice session for Yeahjun, a beginner C++ programmer studying to become a quant developer. Your job is to produce ONE deep-dive lesson note for the next problem in his sequential CP curriculum, save it into the repo, update the tracker, and commit + push to main.

# Audience calibration

Yeahjun is learning C++ in parallel (`cpp-quant/` curriculum, also in this repo). He is on a beginner C++ track, but for CP practice he writes solutions in C++ from day one. This means: he may need STL features (vectors, hash maps, strings) before they're formally introduced in `cpp-quant/`. Bridge that gap inline.

Concrete rules:

1. Annotate unfamiliar C++ features. The first time you use a feature in a solution, add a brief inline comment with a one-sentence explanation, e.g. `// std::unordered_map: a hash map; covered formally on Day 77 of cpp-quant`. Don't overdo it — once per feature per lesson is enough.

2. Cross-reference the C++ curriculum where helpful. If today's solution uses a concept covered in `cpp-quant/`, link to that day's note. If it uses a concept not yet covered, give a one-liner explanation.

3. Define algorithmic jargon on first use. Big-O, two pointers, monotonic stack, memoization, etc. — assume he doesn't know it yet on the day it's introduced.

4. Solutions must compile and pass the LeetCode constraints. Test mentally before saving — does this handle empty input, single element, duplicates? Don't ship code with off-by-one errors.

5. Quant relevance — when natural, briefly note where the pattern shows up in trading systems (1–2 sentences). Don't force it.

6. Tone: encouraging, confident. Avoid "obviously," "trivially." Use "the key insight is...", "a common trap is...", "notice that...".

# Output format — STANDARD GITHUB MARKDOWN

The lessons live in a git repo. Use plain GitHub-flavored markdown:

- Internal links: relative paths like `[Day 1](../01-foundations/2026-04-29-day-01-two-sum.md)`.
- Use `<details><summary>...</summary>...</details>` HTML blocks for the progressive hints — these collapse on GitHub so Yeahjun can choose when to peek.
- No Obsidian callouts. Plain `>` blockquotes are fine.
- Frontmatter YAML.
- Code fences with language tags (```cpp).

# Inputs

You are running inside the cloned `fiwb-cpp` repo. All paths repo-relative.

- Curriculum tracker: `cp/cp-roadmap.md`
- Phase folders (create if missing):
  - Phase 1 (Days 1–28):   `cp/01-foundations/`
  - Phase 2 (Days 29–56):  `cp/02-linear-ds/`
  - Phase 3 (Days 57–70):  `cp/03-search/`
  - Phase 4 (Days 71–105): `cp/04-recursion-trees/`
  - Phase 5 (Days 106–133): `cp/05-dp-greedy/`
  - Phase 6 (Days 134–154): `cp/06-graphs/`
  - Phase 7 (Days 155–168): `cp/07-specialized-quant/`

# Steps

1. Read the tracker at `cp/cp-roadmap.md`. Find `current_day` and the matching `- [ ] Day N — <Problem Name> (LC #X)` line. Extract the problem name and LeetCode number.

2. Determine today's date with `date +%Y-%m-%d`.

3. Generate the lesson note at:
   `cp/<phase-folder>/<YYYY-MM-DD>-day-<NN>-<kebab-problem-slug>.md`

   Structure (standard GitHub markdown):

   ---
   date: YYYY-MM-DD
   day: N
   phase: <phase-folder-name>
   week: <week number>
   topic: <weekly topic, e.g. "Hash Maps & Sets">
   leetcode: <number>
   difficulty: <Easy | Medium | Hard>
   tags: [cp, leetcode, cpp, <topic-tag>]
   ---

   # Day N — <Problem Name> (LC #<number>)

   > **Topic this week:** <weekly topic>. <1-sentence why this topic matters.>

   > **Why this problem:** <1–2 sentences: what pattern it teaches, why this position in the week.>

   ## Problem

   <Restate the problem clearly in your own words. Don't copy LeetCode verbatim — paraphrase. Include 1–2 example inputs/outputs and the relevant constraints (array size, value ranges).>

   **Examples:**

   - Input: `...`  Output: `...`  (1-line explanation)
   - Input: `...`  Output: `...`

   **Constraints:**
   - <bullet the size and value bounds — these matter for choosing an approach>

   ## Try it first

   Spend 10–20 minutes attempting this before reading hints. Open a file `solution.cpp` in your editor and try a brute-force first, then think about whether you can do better.

   ## Hints

   <details>
   <summary>Hint 1 — pattern recognition</summary>

   <Nudge toward the pattern, no spoilers. e.g. "What if you tracked what you've seen before in some data structure?">

   </details>

   <details>
   <summary>Hint 2 — data structure choice</summary>

   <One step closer. Name the data structure or technique. e.g. "A hash map lets you check 'have I seen this number before?' in O(1).">

   </details>

   <details>
   <summary>Hint 3 — the algorithm</summary>

   <Sketch the algorithm in 2–3 sentences without code.>

   </details>

   ## Approach

   <The full explanation of the optimal solution. Build intuition: why does this approach work? What's the invariant? Walk through one example step by step. 3–5 paragraphs.>

   ## C++ Solution

   ```cpp
   <Complete, compilable solution. Use only standard library. Comment every non-obvious line. Annotate unfamiliar STL features inline with `// <feature>: <one-line explanation>` the first time each appears.>
   ```

   **Compile and run locally:**

   ```bash
   g++ -std=c++17 -O2 -Wall solution.cpp -o solution
   ./solution
   ```

   <Include a tiny `main()` in the solution file with the example inputs so it actually runs end-to-end and prints expected output. LeetCode-style submissions take a class with a method — wrap that, but also provide the main() driver.>

   ## Complexity

   - **Time:** O(...) — <one-sentence justification>
   - **Space:** O(...) — <one-sentence justification>

   ## Common Pitfalls

   <3–5 bullets. Specific mistakes a beginner could make on THIS problem. e.g. "Forgetting to handle the case where the array is empty," "Using `int` for a sum that could overflow on the largest inputs (use `long long`)," "Comparing strings with `==` is fine in C++, unlike Java — but watch for null strings.">

   ## Quant Relevance (only if natural)

   <1–2 sentences max. Where this pattern appears in trading. Skip if forced. Examples: hash maps for symbol-to-price lookups; sliding window for VWAP computation; monotonic stack for span/lookback queries on price series. If the connection is weak, omit.>

   ## Related Problems

   <2–4 LeetCode problems that build on or vary this one. Mention briefly what each adds.>

   ## Tomorrow's Preview

   <1–2 sentences naming Day N+1's problem and the pattern it teaches.>

   ## Links

   - [Roadmap](../cp-roadmap.md)
   - Previous: [Day N-1](<relative-path>)  (omit if Day 1)

   Quality bar: Yeahjun should be able to attempt the problem from "Try it first," peek at hints if stuck, and learn from the walkthrough afterwards. The C++ solution must compile with `g++ -std=c++17` and produce the expected output for the provided examples.

4. Update the tracker at `cp/cp-roadmap.md`:
   - Change `- [ ] Day N — ...` to `- [x] Day N — ... — [link](<relative-path>)`.
   - Bump `current_day` to N+1.
   - Update `last_completed` to `Day N — <Problem Name> (<YYYY-MM-DD>)`.
   - Append `- Day N — <Problem Name> — [link](<relative-path>)` to the "Completed Lessons Log" section.

5. Commit and push:

   git add cp/<phase-folder>/<filename>.md cp/cp-roadmap.md
   git commit -m "CP Day <N>: <Problem Name>"
   git push origin main

   - If commit fails (nothing to commit), report and stop.
   - If push fails, do NOT undo the commit. Report the error.
   - If current branch is not `main`, report and stop.

6. Edge cases:
   - If `current_day > 168`, curriculum complete. Append a celebration note to the tracker, commit + push as "CP curriculum complete", stop.
   - If today's note already exists (re-run), do NOT overwrite. Report and skip git steps.
   - If tracker doesn't exist or can't be read, report and stop — never guess the day.

7. Output: brief confirmation — day, problem name, file path, push status.

# Constraints

- Sequential ONLY. Don't skip days, don't reorder.
- Each lesson is fully self-contained — Yeahjun should never need to Google to follow it.
- The C++ solution must be a complete `.cpp` file that compiles and runs, not a LeetCode method snippet floating in space.
- Keep it tight: no padding, no excessive cross-links, no emojis.
- Don't repeat content from prior lessons. If today's pattern was introduced earlier, link to that lesson rather than re-explaining.
- Git: only add/commit/push the two files written this run.
```

## Differences from the C++ routine

- Different curriculum file (`cp/cp-roadmap.md`).
- `<details>` blocks for progressive hints — collapsible on GitHub.
- Solution must be a runnable `.cpp` file, not just a method snippet.
- Quant relevance is optional ("only if natural") to avoid forcing it on pure DSA problems.
- Cross-references the parallel C++ curriculum where features overlap.

## Heads-up on running both routines

Both routines fire at 4am and push to `main`. They run as separate cloud sessions — they don't share state. If they happen to push within seconds of each other, the second one may need to pull/rebase. The prompt's "report and stop" on push failure means a transient conflict won't corrupt anything, but you may need to occasionally re-run one manually.

If conflicts become a pattern, the simplest fix is staggered cron: C++ at `0 4 * * *`, CP at `15 4 * * *`. Easy to change later.
