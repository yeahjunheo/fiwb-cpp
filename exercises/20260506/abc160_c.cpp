#include <algorithm>
#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int K, N;
  std::cin >> K >> N;

  std::vector<int> A(N);
  for (int i = 0; i < N; i++)
    std::cin >> A[i];

  int maxDist = A[0] + (K - A[N - 1]);
  for (int a = 1; a < N; a++)
    maxDist = std::max(maxDist, A[a] - A[a - 1]);

  std::cout << K - maxDist << "\n";
  return 0;
}