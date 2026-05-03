---
date: 2026-05-03
day: 5
phase: 01-foundations
week: 1
topic: "Arrays & Hash Maps"
leetcode: 53
url: https://leetcode.com/problems/maximum-subarray/
difficulty: Medium
tags: [cp, leetcode, cpp, arrays, kadane, dynamic-programming]
---

# Day 5 — Maximum Subarray (LC #53)

**Problem link:** [leetcode.com/problems/maximum-subarray](https://leetcode.com/problems/maximum-subarray/)

> **Topic this week:** Arrays & Hash Maps. Processing arrays in a single left-to-right pass while maintaining a small amount of state is the workhorse technique behind dozens of interview problems.

> **Why this problem:** This is the entry point to dynamic programming on sequences. The algorithm — called Kadane's — is one of the most elegant ideas in competitive programming: a one-liner decision that collapses an O(n²) brute force into O(n). Learn it once, apply it forever.

## Problem

Given an array of integers (which can be negative), find the contiguous subarray — at least one element — that has the largest possible sum, and return that sum.

**Examples:**

- Input: `nums = [-2,1,-3,4,-1,2,1,-5,4]`  Output: `6`  (the subarray `[4,-1,2,1]` sums to 6)
- Input: `nums = [1]`  Output: `1`  (single element; the subarray is just `[1]`)
- Input: `nums = [5,4,-1,7,8]`  Output: `23`  (take the whole array: 5+4-1+7+8=23)

**Constraints:**
- 1 ≤ `nums.length` ≤ 10⁵
- -10⁴ ≤ `nums[i]` ≤ 10⁴

The array can be entirely negative — you must still return the largest (least-negative) single element.

## Try it first

Spend 10–20 minutes before reading the hints. Start with brute force: for each pair `(i, j)`, sum `nums[i..j]` and track the maximum. That works but is O(n²). Then ask yourself: is there information from earlier in the scan that you can reuse instead of recomputing?

## Hints

<details>
<summary>Hint 1 — pattern recognition</summary>

At each index `i`, you face a binary choice: extend the subarray that ended at `i-1`, or throw it away and start a brand-new subarray at `i`. What determines which is better?

</details>

<details>
<summary>Hint 2 — the decision rule</summary>

If the best subarray ending at `i-1` has a **negative** sum, appending it to `nums[i]` would make things worse. You're better off starting fresh at `nums[i]`. So: `current = max(nums[i], current + nums[i])`.

</details>

<details>
<summary>Hint 3 — the algorithm</summary>

Initialize `current = nums[0]` and `best = nums[0]`. For each subsequent element, update `current` with the decision rule above, then update `best = max(best, current)`. One pass, done. Watch your initialization — if you start `best = 0`, you'll get the wrong answer on all-negative arrays.

</details>

## Approach

The key insight is to think about **what's the best subarray ending at position `i`**. Call that value `current`. There are only two candidates: either extend the previous best subarray (value `current + nums[i]`), or discard everything before and start fresh (value `nums[i]`). We pick whichever is larger.

This is Kadane's Algorithm (named after Joseph Kadane, 1984). It's a dynamic programming algorithm where `current` is our DP state — "the maximum subarray sum ending here." We don't need to store the full DP table because each state depends only on the previous one.

Walk through `[-2, 1, -3, 4, -1, 2, 1, -5, 4]`:

| i | nums[i] | current (extend vs restart) | best |
|---|---------|------------------------------|------|
| 0 | -2      | -2 (start)                  | -2   |
| 1 |  1      | max(1, -2+1=-1) = **1**     |  1   |
| 2 | -3      | max(-3, 1-3=-2) = **-2**    |  1   |
| 3 |  4      | max(4, -2+4=2) = **4**      |  4   |
| 4 | -1      | max(-1, 4-1=3) = **3**      |  4   |
| 5 |  2      | max(2, 3+2=5) = **5**       |  5   |
| 6 |  1      | max(1, 5+1=6) = **6**       |  6   |
| 7 | -5      | max(-5, 6-5=1) = **1**      |  6   |
| 8 |  4      | max(4, 1+4=5) = **5**       |  6   |

Answer: **6**, from the subarray `[4,-1,2,1]` (indices 3–6).

Notice that at index 1, we restart because adding -2 to 1 would give -1 — worse than starting at 1 alone. At index 3, we restart again because carrying the -2 forward is a net loss.

A common trap is initializing `best = 0`. That fails on `[-3, -1, -2]` — the correct answer is `-1`, but `best = 0` would (incorrectly) return 0.

## C++ Solution

```cpp
#include <iostream>
#include <vector>   // std::vector: a resizable array; covered formally on Day 5 of cpp-quant
#include <algorithm> // std::max: returns the larger of two values

class Solution {
public:
    int maxSubArray(std::vector<int>& nums) {
        int current = nums[0]; // best subarray sum ending at current index
        int best    = nums[0]; // best sum seen across all ending positions

        for (int i = 1; i < (int)nums.size(); ++i) {
            // Extend previous subarray or restart here — whichever is larger
            current = std::max(nums[i], current + nums[i]);
            best    = std::max(best, current);
        }
        return best;
    }
};

int main() {
    Solution sol;

    std::vector<int> ex1 = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    std::cout << sol.maxSubArray(ex1) << "\n"; // expected: 6

    std::vector<int> ex2 = {1};
    std::cout << sol.maxSubArray(ex2) << "\n"; // expected: 1

    std::vector<int> ex3 = {5, 4, -1, 7, 8};
    std::cout << sol.maxSubArray(ex3) << "\n"; // expected: 23

    std::vector<int> ex4 = {-3, -1, -2}; // all negative: answer is -1
    std::cout << sol.maxSubArray(ex4) << "\n"; // expected: -1

    return 0;
}
```

**Compile and run locally:**

```bash
g++ -std=c++17 -O2 -Wall solution.cpp -o solution
./solution
```

Expected output:
```
6
1
23
-1
```

## Complexity

- **Time:** O(n) — single left-to-right pass over the array.
- **Space:** O(1) — only two integer variables regardless of input size.

## Common Pitfalls

- **Initializing `best = 0` instead of `nums[0]`.** The problem guarantees the array is non-empty and allows all-negative inputs. Starting at 0 gives a wrong answer whenever the true maximum is negative.
- **Starting the loop at index 0 after seeding from index 0.** If you initialize `current = nums[0]` and then loop `for i in 0..n-1`, you process index 0 twice, double-counting it. Start the loop at index 1.
- **Casting `nums.size()` when comparing to `int`.** `std::vector::size()` returns `size_t` (unsigned). Writing `i < nums.size()` with a signed `int i` triggers a signed/unsigned comparison warning on some compilers; cast with `(int)nums.size()` or use `size_t i`.
- **Thinking you need to track subarray boundaries.** The problem only asks for the sum, not the indices. Tracking start/end adds complexity for no benefit here. (If you ever do need the indices, keep a `start` variable that resets to `i` when you restart.)
- **Confusing "subarray" (contiguous) with "subsequence" (non-contiguous).** A non-contiguous version would just sum all positive elements — much easier. Contiguity is what makes Kadane's necessary.

## Quant Relevance

Maximum subarray maps directly to finding the **best contiguous window of returns** in a price series — the stretch of time that maximized cumulative P&L. It also underlies drawdown analysis: the worst subarray sum is the maximum drawdown, solvable by negating the array and running Kadane's.

## Related Problems

- [LC #918 — Maximum Sum Circular Subarray](https://leetcode.com/problems/maximum-sum-circular-subarray/) — Medium — Kadane's on a circular array; the optimal subarray may wrap around the end.
- [LC #152 — Maximum Product Subarray](https://leetcode.com/problems/maximum-product-subarray/) — Medium — Same decision structure, but products require tracking both max and min at each step (negatives flip sign).
- [LC #560 — Subarray Sum Equals K](https://leetcode.com/problems/subarray-sum-equals-k/) — Medium — Finding a subarray with an exact target sum; uses prefix sums + a hash map instead of Kadane's.
- [LC #121 — Best Time to Buy and Sell Stock](https://leetcode.com/problems/best-time-to-buy-and-sell-stock/) — Easy — [Day 2](2026-04-29-day-02-best-time-to-buy-and-sell-stock.md) of this curriculum; the same "track the best seen so far" mindset in a different disguise.

## Tomorrow's Preview

Day 6 tackles **Valid Anagram (LC #242)**, which introduces hash maps for frequency counting — the first problem this week that needs a data structure beyond a plain array.

## Links

- [Roadmap](../cp-roadmap.md)
- Previous: [Day 4 — Product of Array Except Self](2026-05-01-day-04-product-of-array-except-self.md)
