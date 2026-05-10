#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N, D, X;
  std::cin >> N >> D >> X;
  int total = X;
  for (int i = 0; i < N; i++) {
    int a;
    std::cin >> a;
    total += (D - 1) / a + 1;
  }
  std::cout << total << "\n";
}
