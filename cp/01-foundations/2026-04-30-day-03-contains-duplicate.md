---
date: 2026-04-30
day: 3
phase: 01-foundations
week: 1
topic: "Arrays & Hash Maps"
leetcode: 217
url: https://leetcode.com/problems/contains-duplicate/
difficulty: Easy
tags: [cp, leetcode, cpp, hash-set, arrays]
---

# Day 3 — Contains Duplicate (LC #217)

**Problem link:** [leetcode.com/problems/contains-duplicate](https://leetcode.com/problems/contains-duplicate/)

> **Topic this week:** Arrays & Hash Maps. Hash sets answer the question "have I seen this before?" in O(1), which is the building block behind deduplication, frequency counting, and membership tests.

> **Why this problem:** Contains Duplicate is the simplest possible hash-set problem — no arithmetic, no indices, just existence queries. Getting the O(n) set solution right here makes Two Sum (Day 1) and every future "track what you've seen" pattern click into place.

## Problem

Given an integer array `nums`, return `true` if any value appears at least twice in the array, and `false` if every element is distinct.

**Examples:**

- Input: `nums = [1, 2, 3, 1]`  Output: `true`  (1 appears at index 0 and index 3)
- Input: `nums = [1, 2, 3, 4]`  Output: `false`  (all four values are distinct)
- Input: `nums = [1, 1, 1, 3, 3, 4, 3, 2, 4, 2]`  Output: `true`  (several values repeat)

**Constraints:**
- `1 <= nums.length <= 100,000` — O(n²) brute force would be up to 10¹⁰ operations; linear or n log n required
- `-10⁹ <= nums[i] <= 10⁹` — values span the full `int` range, so an array-indexed solution is out

## Try it first

Spend 10–20 minutes attempting this before reading hints. Start with the brute force — two nested loops — and get it working. Then ask: can you answer the same question without re-scanning the whole array every time?

## Hints

<details>
<summary>Hint 1 — pattern recognition</summary>

As you walk through the array from left to right, at each element you need to know: "did I see this exact value earlier?" What if you kept a record of everything you've seen so far?

</details>

<details>
<summary>Hint 2 — data structure choice</summary>

A hash set lets you answer "is X in this collection?" in O(1) on average, with O(1) insertion. You don't need to store positions or counts — just presence. Insert each element; if it's already there, you found a duplicate.

</details>

<details>
<summary>Hint 3 — the algorithm</summary>

Create an empty hash set. Iterate over `nums`. For each element, check if it's already in the set — if yes, return `true`. Otherwise, insert it. If the loop ends without finding a duplicate, return `false`.

</details>

## Approach

The brute-force checks every pair (i, j) with i < j to see if `nums[i] == nums[j]`. That's O(n²) comparisons — about 5 billion operations in the worst case with n = 100,000. It will time out.

A sorting approach works in O(n log n): sort the array, then scan adjacent elements — duplicates become neighbors after sorting. This is worth knowing as an alternative, but it modifies the input and still isn't optimal.

The key insight is: **you only need to know whether you've seen a value before, not where or how many times.** A hash set maintains a collection of unique values, supports O(1) average-case insert and lookup, and uses O(n) space. Walk the array once; the moment you encounter a value already in the set, return `true`.

Let's trace through `[1, 2, 3, 1]`:

| Step | Element | Set before | In set? | Action          |
|------|---------|------------|---------|-----------------|
| 0    | 1       | {}         | no      | insert → {1}    |
| 1    | 2       | {1}        | no      | insert → {1,2}  |
| 2    | 3       | {1,2}      | no      | insert → {1,2,3}|
| 3    | 1       | {1,2,3}    | **yes** | return `true`   |

For `[1, 2, 3, 4]` the loop completes without a hit and we return `false`.

A common trap is checking *after* inserting. If you do `seen.insert(x); if (seen.count(x) > 1)` you'll never detect anything because sets don't store duplicates — the second insert of a duplicate is silently ignored. Always check *before* inserting.

## C++ Solution

```cpp
#include <iostream>
#include <vector>
#include <unordered_set>  // std::unordered_set: hash set with O(1) avg insert/lookup; covered formally on Day 78 of cpp-quant

bool containsDuplicate(const std::vector<int>& nums) {
    std::unordered_set<int> seen;  // stores values encountered so far
    for (int x : nums) {           // range-based for: iterates over each element; covered formally on Day 45 of cpp-quant
        if (seen.count(x)) return true;  // .count(x): 1 if x is present, 0 otherwise
        seen.insert(x);
    }
    return false;
}

int main() {
    // Example 1: expected true
    std::vector<int> nums1 = {1, 2, 3, 1};
    std::cout << (containsDuplicate(nums1) ? "true" : "false") << "\n";

    // Example 2: expected false
    std::vector<int> nums2 = {1, 2, 3, 4};
    std::cout << (containsDuplicate(nums2) ? "true" : "false") << "\n";

    // Example 3: expected true
    std::vector<int> nums3 = {1, 1, 1, 3, 3, 4, 3, 2, 4, 2};
    std::cout << (containsDuplicate(nums3) ? "true" : "false") << "\n";

    // Edge case: single element — cannot have a duplicate
    std::vector<int> nums4 = {42};
    std::cout << (containsDuplicate(nums4) ? "true" : "false") << "\n";

    // Edge case: two identical elements — smallest possible duplicate
    std::vector<int> nums5 = {-1000000000, -1000000000};
    std::cout << (containsDuplicate(nums5) ? "true" : "false") << "\n";

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
true
false
true
false
true
```

## Complexity

- **Time:** O(n) — one pass over the array; each `count` and `insert` is O(1) average for `unordered_set`
- **Space:** O(n) — in the worst case (all distinct elements) the set holds n entries

> **Alternative — sorting:** O(n log n) time, O(log n) space (in-place sort). Sort `nums`, then check `nums[i] == nums[i-1]` for each i from 1 to n-1. Useful if you cannot allocate extra space, but modifies the input array.

## Common Pitfalls

- **Checking after inserting.** `std::unordered_set` silently ignores duplicate inserts — calling `.count()` after `.insert()` will always return 1 or less. Check presence *before* inserting.
- **Assuming values fit in an array index.** Values go up to 10⁹ — you cannot allocate a boolean array of that size. The hash set works for any integer range.
- **Using `std::set` instead of `std::unordered_set`.** `std::set` is a sorted tree with O(log n) operations. It gives the correct answer but runs slower. For existence-only queries with no ordering needed, `unordered_set` is the right choice.
- **Forgetting the early-return.** Return `true` as soon as you find a duplicate — don't keep scanning. This is both correct and efficient (a duplicate near the front exits in O(1)).
- **Empty array.** The constraint guarantees `nums.length >= 1`, but the algorithm is safe anyway — an empty loop returns `false`, which is correct.

## Quant Relevance

Exchanges and trading systems assign unique IDs to every order. On arrival, an order ID is looked up in an `unordered_set`; a hit means a replay or duplicate submission, which is rejected immediately. The same O(1) deduplication pattern appears in feed handlers filtering repeated market-data messages.

## Related Problems

- [LC #219 — Contains Duplicate II](https://leetcode.com/problems/contains-duplicate-ii/) — Easy — same problem but duplicates must be within `k` index distance of each other; sliding-window variant of the hash-set approach
- [LC #220 — Contains Duplicate III](https://leetcode.com/problems/contains-duplicate-iii/) — Hard — duplicates must be within both index distance k *and* value distance t; requires a sorted structure like `std::set` for the range query
- [LC #1 — Two Sum](https://leetcode.com/problems/two-sum/) — Easy — same "track what you've seen" idea but with a hash *map* to also record indices; covered on [Day 1](2026-04-29-day-01-two-sum.md)
- [LC #128 — Longest Consecutive Sequence](https://leetcode.com/problems/longest-consecutive-sequence/) — Medium — builds on the hash-set membership test to find runs of consecutive integers in O(n); covered on Day 55

## Tomorrow's Preview

Day 4 is **Product of Array Except Self** (LC #238), which introduces the prefix-product pattern — computing an output array where each position holds the product of all *other* elements, without using division, in O(n).

## Links

- [Roadmap](../cp-roadmap.md)
- Previous: [Day 2 — Best Time to Buy and Sell Stock](2026-04-29-day-02-best-time-to-buy-and-sell-stock.md)
