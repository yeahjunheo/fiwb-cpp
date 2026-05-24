#include <cmath>
#include <iostream>
#include <vector>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N, D;
  std::cin >> N >> D;
  std::vector<std::vector<int>> points(N, std::vector<int>(D));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < D; j++) {
      std::cin >> points[i][j];
    }
  }
  int count = 0;
  for (int i = 0; i < N; i++) {
    for (int j = i + 1; j < N; j++) {
      int distance = 0;
      for (int k = 0; k < D; k++) {
        int diff = points[i][k] - points[j][k];
        distance += diff * diff;
      }
      long long root = round(sqrtl((long double)distance));
      count += (root * root == distance);
    }
  }
  std::cout << count << '\n';
}
