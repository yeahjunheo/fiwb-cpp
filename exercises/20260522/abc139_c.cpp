#include <algorithm>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  int prev;
  std::cin >> prev;
  int run = 0, maxRun = 0;
  for (int i = 1; i < N; i++) {
    int h;
    std::cin >> h;
    run = (h > prev) ? 0 : run + 1;
    maxRun = std::max(maxRun, run);
    prev = h;
  }
  std::cout << maxRun << "\n";
}
