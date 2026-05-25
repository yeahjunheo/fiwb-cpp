#include <climits>
#include <cstdlib>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N, T, A;
  std::cin >> N >> T >> A;
  int minTempDiff = INT_MAX, returnIdx = 0;
  for (int i = 0; i < N; i++) {
    int H;
    std::cin >> H;
    int temp = 1000 * T - H * 6;
    int tempDiff = abs(1000 * A - temp);
    if (tempDiff < minTempDiff) {
      minTempDiff = tempDiff;
      returnIdx = i + 1;
    }
  }
  std::cout << returnIdx << '\n';
}
