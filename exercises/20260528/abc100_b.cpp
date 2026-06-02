#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int D, N;
  std::cin >> D >> N;
  long long baseFactor = (D == 0 ? 1 : D == 1 ? 100 : 10000);
  long long K = (N == 100) ? 101 : N;
  std::cout << K * baseFactor << '\n';
}
