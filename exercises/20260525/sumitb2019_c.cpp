#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int X;
  std::cin >> X;
  const int upperBound = X / 100;
  const int leftOver = X % 100;
  std::cout << (leftOver <= 5 * upperBound ? 1 : 0) << '\n';
}
