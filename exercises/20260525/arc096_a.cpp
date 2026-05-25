#include <algorithm>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int A, B, C, X, Y;
  std::cin >> A >> B >> C >> X >> Y;
  int bestA = std::min(A, C * 2);
  int bestB = std::min(B, C * 2);
  int bestC = std::min(A + B, C * 2);
  int total = 0;
  if (X >= Y) {
    total += bestC * Y + bestA * (X - Y);
  } else {
    total += bestC * X + bestB * (Y - X);
  }
  std::cout << total << '\n';
}
