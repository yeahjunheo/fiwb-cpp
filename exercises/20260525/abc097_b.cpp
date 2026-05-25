#include <algorithm>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int X;
  std::cin >> X;
  int best = 1;
  for (int b = 2; b * b <= X; b++) {
    long long power = (long long)b * b;
    while (power <= X) {
      best = std::max(best, (int)power);
      power *= b;
    }
  }
  std::cout << best << '\n';
}
