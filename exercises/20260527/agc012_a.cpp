#include <algorithm>
#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  std::vector<int> strengths(3 * N);
  for (int i = 0; i < 3 * N; i++) {
    std::cin >> strengths[i];
  }
  std::sort(strengths.begin(), strengths.end());
  long long totalSum = 0;
  for (int i = N; i < 3 * N; i += 2) {
    totalSum += strengths[i];
  }
  std::cout << totalSum << '\n';
}
