#include <algorithm>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  int prevVal;
  std::cin >> prevVal;
  int sum = prevVal;
  for (int i = 1; i < N - 1; i++) {
    int b;
    std::cin >> b;
    sum += std::min(prevVal, b);
    prevVal = b;
  }
  sum += prevVal;
  std::cout << sum << '\n';
}
