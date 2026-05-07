#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  std::vector<int> V(N);
  for (int i = 0; i < N; i++)
    std::cin >> V[i];
  std::sort(V.begin(), V.end());
  double res = V[0];
  for (int i = 1; i < N; i++) {
    res = (res + V[i]) / 2;
  }
  std::cout << std::fixed << std::setprecision(10) << res << '\n';
}
