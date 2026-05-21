# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

**Also read `AGENTS.md`** — it defines the interactive workflows (especially the competitive-programming problem workflow) that apply across every session.

@AGENTS.md

## Repo purpose

Personal practice log for a 168-day quant-developer curriculum. Two parallel tracks generate one Markdown lesson per day; daily scratch C++ code lives in dated folders. There is no application to build or test suite to run — the repo is a notebook plus throwaway exercises.

## Daily workflow (driven by a scheduled task)

A scheduled Claude Code task runs at 4 AM and, **for each track**:

1. Reads `current_day` from the track's roadmap file.
2. Generates that day's lesson as a new Markdown note in the matching `NN-phase/` folder, named `YYYY-MM-DD-day-NN-<slug>.md`.
3. Marks the day's checkbox `[x]` in the roadmap, updates `last_completed`, and increments `current_day`.

The two roadmaps are the source of truth — never edit a lesson without updating the roadmap state, and never advance `current_day` without producing the lesson file.

| Track | Roadmap | Lesson format |
|-------|---------|---------------|
| C++ language (`cpp-quant/`) | `cpp-quant/cpp-roadmap.md` | Concept → Code Example → **Quant Application** → Pitfalls → Exercise → Tomorrow's Preview |
| Competitive programming (`cp/`) | `cp/cp-roadmap.md` | Problem → Try it first → Solution(s) → Pattern → Complexity (LeetCode-linked) |

Lesson files use YAML frontmatter (`date`, `day`, `phase`, `tags`, plus track-specific fields like `leetcode`/`difficulty` for `cp/`). Match the structure of existing notes in the same phase folder rather than inventing a new shape.

## Daily scratch code (`YYYYMMDD/`)

Ad-hoc C++ files for hands-on exercises tied to that day's lesson. Compile directly with the system compiler — there is no build system, Makefile, or CMake:

```bash
g++ -std=c++20 -Wall -Wextra file.cpp -o file
# multi-file:
g++ -std=c++20 -Wall -Wextra main.cpp greet.cpp -o main
```

Compiled binaries (e.g. `20260429/types`, `20260429/overflow`) are checked in alongside sources — that's intentional for this repo, don't `.gitignore` them unless asked.

## Conventions when generating content

- **Date stamps use the curriculum calendar** (start_date `2026-04-29`), not necessarily today's wall-clock date — read it from the roadmap.
- Lesson filenames: `YYYY-MM-DD-day-NN-<kebab-slug>.md`, zero-padded day.
- When adding a lesson, the *only* roadmap edits are: tick the checkbox, append `— [link](path)`, bump `current_day`, update `last_completed`. Don't restructure the roadmap.
- Phase folders are pre-created and numbered (`01-foundations/` … `07-quant-specific/` or `07-specialized-quant/`); place each lesson in the folder matching the day's phase per the roadmap.

## When to use web tools during lesson generation

`WebFetch` and `WebSearch` are pre-allowed in this repo. Reach for them when being wrong has a cost — not for things you already know.

- **Fetch authoritative C++ references** before writing standard-library or language-feature lessons (cppreference.com pages for the specific feature; standard wording when the lesson hinges on it).
- **Verify the LeetCode problem** when generating a `cp/` lesson — pull the current problem statement, constraints, and difficulty rather than recalling them.
- **Confirm quant-application claims** before stating them — if a lesson asserts how HFT systems, exchanges, or matching engines use a feature, ground it in a real source instead of plausible-sounding generality.
- **Check compiler/intrinsic specifics** (GCC/Clang flag behavior, `-march` targets, x86 intrinsics) since these drift between versions.

Skip web tools for foundational concepts already covered well by the curriculum's lesson format — generic "what is a hash map" prose adds no value.
