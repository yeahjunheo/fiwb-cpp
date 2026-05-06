#include <algorithm>
#include <cstdlib>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N, K;
  std::cin >> N >> K;
  int minDist = 0;
  while (N--) {
    int x;
    std::cin >> x;
    int bx = K - x;
    minDist += 2 * std::min(x, bx);
  }

  std::cout << minDist << "\n";
  return 0;
}