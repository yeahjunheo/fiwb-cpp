#include <algorithm>
#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  std::vector<int> row1(N), row2(N);
  for (int i = 0; i < N; i++)
    std::cin >> row1[i];
  for (int i = 0; i < N; i++)
    std::cin >> row2[i];

  int topSum = row1[0];
  int dp = row1[0] + row2[0];
  for (int i = 1; i < N; i++) {
    topSum += row1[i];
    dp = std::max(dp, topSum) + row2[i];
  }
  std::cout << dp << '\n';
}
