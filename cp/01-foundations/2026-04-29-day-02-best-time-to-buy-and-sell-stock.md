---
date: 2026-04-29
day: 2
phase: 01-foundations
week: 1
topic: "Arrays & Hash Maps"
leetcode: 121
url: https://leetcode.com/problems/best-time-to-buy-and-sell-stock/
difficulty: Easy
tags: [cp, leetcode, cpp, arrays, greedy, sliding-window]
---

# Day 2 — Best Time to Buy and Sell Stock (LC #121)

**Problem link:** [leetcode.com/problems/best-time-to-buy-and-sell-stock](https://leetcode.com/problems/best-time-to-buy-and-sell-stock/)

> **Topic this week:** Arrays & Hash Maps. Knowing how to extract a single-pass answer from an array is the bedrock of every performance-sensitive algorithm.

> **Why this problem:** It's the canonical "track one running value while scanning" pattern — once you see it here, you'll recognize it immediately in harder problems like Maximum Subarray and sliding-window variants later in the week.

## Problem

You're given an array `prices` where `prices[i]` is the price of a stock on day `i`. You may buy on one day and sell on a later day (you must buy before you sell). Return the maximum profit you can achieve. If no profit is possible, return `0`.

**Examples:**

- Input: `prices = [7, 1, 5, 3, 6, 4]`  Output: `5`  (buy at 1 on day 1, sell at 6 on day 4)
- Input: `prices = [7, 6, 4, 3, 1]`  Output: `0`  (prices only fall — no profitable trade exists)

**Constraints:**
- `1 <= prices.length <= 100,000` — a single linear pass must be sufficient; an O(n²) brute force will time-out
- `0 <= prices[i] <= 10,000` — values fit in a regular `int`; no overflow risk here

## Try it first

Spend 10–20 minutes on this before reading hints. The brute-force approach is: try every pair (buy day, sell day) and keep the best. Can you do it in a single pass?

## Hints

<details>
<summary>Hint 1 — pattern recognition</summary>

As you scan left to right, at each price you can ask: "if I sold today, what's the best I could have done?" What information do you need to answer that question quickly?

</details>

<details>
<summary>Hint 2 — data structure choice</summary>

You only need one extra variable: the minimum price seen so far. At each index, `current price − minimum so far` is the profit if you sell today. Keep updating the best profit as you go.

</details>

<details>
<summary>Hint 3 — the algorithm</summary>

Initialize `min_price = prices[0]` and `max_profit = 0`. Iterate from index 1 to the end. At each step, update `min_price = min(min_price, prices[i])`, then update `max_profit = max(max_profit, prices[i] - min_price)`. Return `max_profit`.

</details>

## Approach

The brute-force approach checks every pair of days (buy on day `i`, sell on day `j > i`) for a profit of `prices[j] - prices[i]`, keeping the maximum. That's O(n²) time — it works for tiny inputs but fails the 100,000-element constraint.

The key insight is: **when you're standing at day `j` deciding whether to sell, the best possible profit is `prices[j] minus the cheapest price seen on any day before j`.** You don't need to remember all previous prices — just the minimum so far.

This gives a clean one-pass solution. Walk through the array from left to right, maintaining two variables: the cheapest buy price seen so far, and the best profit seen so far. At each position you do two quick updates and move on.

Let's trace through `[7, 1, 5, 3, 6, 4]`:

| Day | Price | min_price | profit today | max_profit |
|-----|-------|-----------|--------------|------------|
| 0   | 7     | 7         | —            | 0          |
| 1   | 1     | 1         | 1−1 = 0      | 0          |
| 2   | 5     | 1         | 5−1 = 4      | 4          |
| 3   | 3     | 1         | 3−1 = 2      | 4          |
| 4   | 6     | 1         | 6−1 = 5      | 5          |
| 5   | 4     | 1         | 4−1 = 3      | 5          |

Answer: 5. Notice that we never need to go back — `min_price` absorbs all history we need.

A common trap is updating `min_price` and then immediately computing the profit using the just-updated `min_price`. The order is: update `min_price` first, then compute profit from the current price. This is safe because we're computing `prices[i] - min_price`, and the updated `min_price` is at most `prices[i]`, giving a profit of zero — which is correct (you can't profit by buying and selling on the same day).

## C++ Solution

```cpp
#include <iostream>
#include <vector>
#include <algorithm>  // std::min, std::max: covered formally on Day 4 of cpp-quant

int maxProfit(const std::vector<int>& prices) {  // std::vector<int>: a resizable array; covered formally on Day 20 of cpp-quant
    if (prices.empty()) return 0;

    int min_price = prices[0];
    int max_profit = 0;

    for (int i = 1; i < (int)prices.size(); ++i) {
        min_price = std::min(min_price, prices[i]);
        max_profit = std::max(max_profit, prices[i] - min_price);
    }

    return max_profit;
}

int main() {
    // Example 1: expected output 5
    std::vector<int> prices1 = {7, 1, 5, 3, 6, 4};
    std::cout << maxProfit(prices1) << "\n";  // 5

    // Example 2: expected output 0
    std::vector<int> prices2 = {7, 6, 4, 3, 1};
    std::cout << maxProfit(prices2) << "\n";  // 0

    // Edge case: single element — no transaction possible
    std::vector<int> prices3 = {42};
    std::cout << maxProfit(prices3) << "\n";  // 0

    // Edge case: two elements, profit possible
    std::vector<int> prices4 = {2, 7};
    std::cout << maxProfit(prices4) << "\n";  // 5

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
5
0
0
5
```

## Complexity

- **Time:** O(n) — we visit each price exactly once
- **Space:** O(1) — only two integers regardless of input size

## Common Pitfalls

- **Returning a negative profit.** If prices only decrease, `prices[i] - min_price` will always be 0 (you'd buy and sell on the same day — no gain). Initializing `max_profit = 0` handles this correctly; never let it go negative.
- **Off-by-one: starting the loop at index 0.** You can't sell on day 0 (no prior day to buy). Start `i` at 1. The cast `(int)prices.size()` avoids a signed/unsigned comparison warning that `-Wall` will flag.
- **Buying after selling.** The constraint is sell strictly after buy. Because `min_price` is always from a previous index (or the same, giving 0 profit), this is automatically satisfied.
- **Empty array.** LeetCode guarantees `prices.length >= 1`, but defensive code is a habit worth building. The `if (prices.empty()) return 0;` guard costs nothing.
- **Thinking you need two passes.** A common first instinct is to find the global minimum, then find the maximum after it. That's correct but unnecessary — one pass does both simultaneously.

## Quant Relevance

This is literally the P&L maximization problem for a single-trade strategy on a price series. In trading systems, the same "track the running minimum" technique appears in drawdown calculation — the maximum peak-to-trough decline in a portfolio's value.

## Related Problems

- [LC #53 — Maximum Subarray](https://leetcode.com/problems/maximum-subarray/) — Easy — Kadane's algorithm is the same "running best" idea applied to contiguous sums; covered on Day 5
- [LC #122 — Best Time to Buy and Sell Stock II](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-ii/) — Medium — allows unlimited transactions; greedy approach
- [LC #123 — Best Time to Buy and Sell Stock III](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iii/) — Hard — at most 2 transactions; requires DP; covered on Day 126
- [LC #309 — Best Time to Buy and Sell Stock with Cooldown](https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-cooldown/) — Medium — adds a rest-day constraint; state machine DP; covered on Day 124

## Tomorrow's Preview

Day 3 is **Contains Duplicate** (LC #217), which introduces the hash set — a data structure that answers "have I seen this before?" in O(1) time. It's a direct counterpart to today's running-minimum trick, but for existence queries.

## Links

- [Roadmap](../cp-roadmap.md)
- Previous: [Day 1 — Two Sum](2026-04-29-day-01-two-sum.md)
