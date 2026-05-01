---
date: 2026-05-01
day: 4
phase: 01-foundations
week: 1
topic: "Arrays & Hash Maps"
leetcode: 238
url: https://leetcode.com/problems/product-of-array-except-self/
difficulty: Medium
tags: [cp, leetcode, cpp, arrays, prefix-product]
---

# Day 4 — Product of Array Except Self (LC #238)

**Problem link:** [leetcode.com/problems/product-of-array-except-self](https://leetcode.com/problems/product-of-array-except-self/)

> **Topic this week:** Arrays & Hash Maps. Precomputing cumulative state while scanning is one of the most reusable patterns in competitive programming — it turns an O(n²) repeated scan into an O(n) single pass.

> **Why this problem:** This is the canonical "prefix array" problem. It forces you to think about what information you can precompute in one direction and then reuse while scanning in the other. The trick generalises far beyond products.

## Problem

Given an integer array `nums`, return an array `answer` where `answer[i]` equals the product of every element in `nums` except `nums[i]` itself.

**You must not use division, and the solution must run in O(n) time.**

**Examples:**

- Input: `nums = [1,2,3,4]`  Output: `[24,12,8,6]`  (e.g. answer[1] = 1×3×4 = 12)
- Input: `nums = [-1,1,0,-3,3]`  Output: `[0,0,9,0,0]`  (the zero makes every product except index 2 zero; answer[2] = -1×1×-3×3 = 9)

**Constraints:**
- `2 <= nums.length <= 100,000` — O(n²) nested-loop brute force has 10¹⁰ operations at the upper bound; O(n) is required
- `-30 <= nums[i] <= 30` — values are small, but an array of 100,000 elements each equal to 30 would overflow if you tried to take the total product and divide; the "no division" constraint is not just artificial
- The product of any prefix or suffix fits in a 32-bit integer — `int` is safe for partial products

## Try it first

Spend 10–20 minutes before reading hints. Start with the naive O(n²) approach — nested loops, multiply everything except index `i` — and get it returning the right answer. Then ask: am I repeating work? What does each inner-loop pass compute that overlaps with the previous one?

## Hints

<details>
<summary>Hint 1 — pattern recognition</summary>

For index `i`, you need the product of elements to its left and the product of elements to its right. Those two groups don't overlap. What if you computed each group separately?

</details>

<details>
<summary>Hint 2 — data structure choice</summary>

You can precompute a "prefix product" array where `prefix[i]` = product of `nums[0..i-1]`, and a "suffix product" array where `suffix[i]` = product of `nums[i+1..n-1]`. Then `answer[i] = prefix[i] * suffix[i]`. Can you do this with O(1) extra space instead of two arrays?

</details>

<details>
<summary>Hint 3 — the algorithm</summary>

Do two passes over `nums`. In the first pass (left to right), fill `answer[i]` with the running product of everything to the left of `i`. In the second pass (right to left), multiply each `answer[i]` by a running suffix product you maintain in a single variable. The output array itself is the only extra space used.

</details>

## Approach

The key insight is that for any index `i`:

```
answer[i] = (product of nums[0] through nums[i-1]) × (product of nums[i+1] through nums[n-1])
           = left_product[i]                         × right_product[i]
```

Those two halves are independent — the left product only cares about elements before `i`, the right product only cares about elements after `i`.

**Naive approach (O(n²)):** For each `i`, loop over all other indices and multiply. This is the obvious baseline but fails on large inputs.

**Prefix + suffix arrays (O(n), O(n) space):** Build two auxiliary arrays. `prefix[i]` = product of elements to the left of `i` (so `prefix[0] = 1` since there are no elements to the left). `suffix[i]` = product of elements to the right of `i` (so `suffix[n-1] = 1`). Then `answer[i] = prefix[i] * suffix[i]`. Two linear passes, two extra arrays.

**Optimal (O(n), O(1) extra space):** The trick is to reuse the output array itself as the prefix accumulator, then apply the suffix in a second pass with a single variable.

Walk through `[1, 2, 3, 4]` step by step:

**Pass 1 — left to right (building prefix into `answer`):**

| i | prefix (before) | answer[i] | prefix (after) |
|---|-----------------|-----------|----------------|
| 0 | 1               | 1         | 1×1 = 1        |
| 1 | 1               | 1         | 1×2 = 2        |
| 2 | 2               | 2         | 2×3 = 6        |
| 3 | 6               | 6         | 6×4 = 24       |

After pass 1: `answer = [1, 1, 2, 6]` — each slot holds the product of everything to its left.

**Pass 2 — right to left (multiplying in suffix):**

| i | suffix (before) | answer[i]  | suffix (after) |
|---|-----------------|------------|----------------|
| 3 | 1               | 6×1 = 6    | 1×4 = 4        |
| 2 | 4               | 2×4 = 8    | 4×3 = 12       |
| 1 | 12              | 1×12 = 12  | 12×2 = 24      |
| 0 | 24              | 1×24 = 24  | 24×1 = 24      |

Final: `answer = [24, 12, 8, 6]` — correct.

Notice that zeros work naturally: if `nums[i] = 0`, the suffix variable becomes 0 after passing through index `i`, zeroing out all answers to its left (as expected, since all those products include a zero). No special-casing needed.

## C++ Solution

```cpp
#include <iostream>
#include <vector>

class Solution {
public:
    std::vector<int> productExceptSelf(std::vector<int>& nums) {
        int n = static_cast<int>(nums.size());
        std::vector<int> answer(n, 1); // std::vector: dynamic array; covered formally on Day 5 of cpp-quant

        // Pass 1: answer[i] = product of all elements to the LEFT of i
        int prefix = 1;
        for (int i = 0; i < n; ++i) {
            answer[i] = prefix;
            prefix *= nums[i];
        }

        // Pass 2: multiply in product of all elements to the RIGHT of i
        int suffix = 1;
        for (int i = n - 1; i >= 0; --i) {
            answer[i] *= suffix;
            suffix *= nums[i];
        }

        return answer;
    }
};

int main() {
    Solution sol;

    // Example 1
    std::vector<int> nums1 = {1, 2, 3, 4};
    auto ans1 = sol.productExceptSelf(nums1);
    std::cout << "Example 1: [";
    for (int i = 0; i < static_cast<int>(ans1.size()); ++i) {
        if (i > 0) std::cout << ",";
        std::cout << ans1[i];
    }
    std::cout << "]\n"; // expected: [24,12,8,6]

    // Example 2
    std::vector<int> nums2 = {-1, 1, 0, -3, 3};
    auto ans2 = sol.productExceptSelf(nums2);
    std::cout << "Example 2: [";
    for (int i = 0; i < static_cast<int>(ans2.size()); ++i) {
        if (i > 0) std::cout << ",";
        std::cout << ans2[i];
    }
    std::cout << "]\n"; // expected: [0,0,9,0,0]

    return 0;
}
```

**Compile and run locally:**

```bash
g++ -std=c++17 -O2 -Wall solution.cpp -o solution
./solution
```

## Complexity

- **Time:** O(n) — two linear passes, each visiting every element exactly once
- **Space:** O(1) extra — the output array itself is not counted; `prefix` and `suffix` are single integers

## Common Pitfalls

- **Initialising `prefix` and `suffix` to 0 instead of 1.** The identity element for multiplication is 1 (multiplying by 1 leaves the product unchanged). Starting at 0 would zero out everything.
- **Forgetting to update the running product after using it.** The pattern is: write `answer[i] = prefix`, *then* `prefix *= nums[i]` — not before. Reversing these two lines produces wrong answers because index `i` would include itself in its own product.
- **Worrying about integer overflow.** The constraints guarantee prefix and suffix products fit in 32-bit integers, so plain `int` is safe. On a problem without that guarantee, you'd switch to `long long`.
- **Trying to handle zeros specially.** A common first instinct is to count zeros and special-case. You don't need to — the prefix/suffix approach handles zeros automatically: a zero in `nums` propagates through the running product exactly as intended.
- **Attempting the division shortcut.** `total_product / nums[i]` seems elegant but breaks when any element is zero (division by zero), and the problem explicitly forbids it anyway.

## Quant Relevance

In portfolio attribution, computing the "return contribution of all assets except asset X" is exactly this pattern applied to factor returns — a running product from the left, multiplied by a running product from the right. The same prefix-suffix decomposition also underlies efficiency optimisations in options path-simulation where per-step multipliers must be accumulated without recomputing from scratch at every time step.

## Related Problems

- [LC #724 — Find Pivot Index](https://leetcode.com/problems/find-pivot-index/) — Easy — the same "prefix from left, suffix from right" pattern applied to sums instead of products; a good warm-up if the product version felt hard
- [LC #152 — Maximum Product Subarray](https://leetcode.com/problems/maximum-product-subarray/) — Medium — prefix products with a twist: negative numbers can flip max to min, so you must track both simultaneously
- [LC #42 — Trapping Rain Water](https://leetcode.com/problems/trapping-rain-water/) — Hard — precompute `max_left[i]` and `max_right[i]` arrays (same structural idea, different operation), then combine in a final pass
- [LC #1352 — Product of the Last K Numbers](https://leetcode.com/problems/product-of-the-last-k-numbers/) — Medium — online version: a stream of numbers arrives and you must answer "product of last k" queries in O(1) using a running prefix-product list

## Tomorrow's Preview

Day 5 is Maximum Subarray (LC #53), the classic introduction to **Kadane's algorithm** — a single-pass greedy/DP technique for finding the contiguous subarray with the largest sum. It teaches the "carry-forward or restart" decision that appears throughout 1D dynamic programming.

## Links

- [Roadmap](../cp-roadmap.md)
- Previous: [Day 3 — Contains Duplicate](2026-04-30-day-03-contains-duplicate.md)
