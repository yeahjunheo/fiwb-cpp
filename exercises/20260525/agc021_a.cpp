#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  long long N;
  std::cin >> N;
  std::string s = std::to_string(N);
  int best = 0;
  int sum = 0;
  for (char c : s)
    sum += c - '0';
  best = sum;
  for (size_t i = 0; i < s.size(); i++) {
    if (s[i] == '0')
      continue;
    int cand = 0;
    for (size_t j = 0; j < i; j++)
      cand += s[j] - '0';
    cand += s[i] - '0' - 1;
    cand += 9 * (s.size() - i - 1);
    best = std::max(best, cand);
  }
  std::cout << best << '\n';
}
