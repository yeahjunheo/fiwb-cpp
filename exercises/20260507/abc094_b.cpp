#include <algorithm>
#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N, M, X;
  std::cin >> N >> M >> X;

  std::vector<int> pre(N + 1, 0);
  for (int i = 0; i < M; i++) {
    int p;
    std::cin >> p;
    pre[p] = 1;
  }
  for (int i = 1; i <= N; i++)
    pre[i] += pre[i - 1];

  int leftToll = pre[X - 1];
  int rightToll = pre[N - 1] - pre[X];
  std::cout << std::min(leftToll, rightToll) << "\n";
}
