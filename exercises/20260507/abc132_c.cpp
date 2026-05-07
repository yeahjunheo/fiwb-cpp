#include <algorithm>
#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  std::vector<int> D(N);
  for (int i = 0; i < N; i++)
    std::cin >> D[i];
  // std::sort(D.begin(), D.end());
  // std::cout << D[N / 2] - D[N / 2 - 1] << "\n";
  std::nth_element(D.begin(), D.begin() + N / 2 - 1, D.end());
  int low = D[N / 2 - 1];
  int high = *std::min_element(D.begin() + N / 2, D.end());
  std::cout << high - low << "\n";
}
