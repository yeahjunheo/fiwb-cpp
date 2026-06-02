#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  int totalOps = 0;
  for (int i = 0; i < N; i++) {
    int a;
    std::cin >> a;
    while (a > 0 && a % 2 == 0) {
      totalOps++;
      a /= 2;
    }
  }
  std::cout << totalOps << '\n';
}
