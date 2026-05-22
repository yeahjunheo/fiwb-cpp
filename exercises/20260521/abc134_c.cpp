#include <algorithm>
#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  std::vector<int> A(N);
  int maxA = 0;
  int secondMax = 0;
  int count = 0;
  for (int i = 0; i < N; i++) {
    int x;
    std::cin >> x;
    A[i] = x;
    if (x > maxA) {
      secondMax = maxA;
      maxA = x;
      count = 1;
    } else if (x == maxA) {
      count++;
    } else {
      secondMax = std::max(secondMax, x);
    }
  }
  for (int x : A) {
    if (x == maxA && count == 1)
      std::cout << secondMax << "\n";
    else
      std::cout << maxA << "\n";
  }
}
