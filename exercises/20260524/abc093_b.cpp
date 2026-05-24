#include <algorithm>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int A, B, K;
  std::cin >> A >> B >> K;
  int firstBound = std::min(A + K - 1, B);
  for (int i = A; i <= firstBound; i++) {
    std::cout << i << '\n';
  }
  int secondBound = std::max(A + K, B - K + 1);
  for (int i = secondBound; i <= B; i++) {
    std::cout << i << '\n';
  }
}
