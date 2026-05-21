#include <cstdlib>
#include <iostream>
#include <vector>

// ---- Approach 1: brute force with std::next_permutation ----
// Walk all N! permutations in lex order; record the rank when we hit P or Q.
// Works because N <= 8 (8! = 40320).
//
// int main() {
//   std::ios_base::sync_with_stdio(false);
//   std::cin.tie(nullptr);
//
//   int N;
//   std::cin >> N;
//   std::vector<int> P(N), Q(N);
//   for (int i = 0; i < N; i++) std::cin >> P[i];
//   for (int i = 0; i < N; i++) std::cin >> Q[i];
//
//   std::vector<int> perm(N);
//   for (int i = 0; i < N; i++) perm[i] = i + 1;
//
//   int a = 0, b = 0, rank = 1;
//   do {
//     if (perm == P) a = rank;
//     if (perm == Q) b = rank;
//     rank++;
//   } while (std::next_permutation(perm.begin(), perm.end()));
//
//   std::cout << std::abs(a - b) << "\n";
// }

// ---- Approach 2: Lehmer code / factorial number system ----
// Compute each permutation's rank directly, in O(N^2), without enumerating
// every permutation. Idea: at position i, count how many of the still-unused
// values are smaller than perm[i]; that many entire "blocks" of (N-1-i)!
// permutations come before us at this prefix. Sum those contributions.
//
// We can do this while reading the input — no need to store the permutation.

int factorial(int n) {
  int result = 1;
  for (int i = 2; i <= n; i++) result *= i;
  return result;
}

// Read N values from stdin and return their 1-indexed lex rank.
int read_and_rank(int N) {
  std::vector<bool> used(N + 1, false);  // used[v] = true once value v has appeared
  int rank = 0;                          // 0-indexed for now
  for (int i = 0; i < N; i++) {
    int v;
    std::cin >> v;
    int smaller = 0;                     // unused values strictly less than v
    for (int j = 1; j < v; j++)
      if (!used[j]) smaller++;
    rank += smaller * factorial(N - 1 - i);
    used[v] = true;
  }
  return rank + 1;                       // convert to 1-indexed
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  int a = read_and_rank(N);
  int b = read_and_rank(N);
  std::cout << std::abs(a - b) << "\n";
}
