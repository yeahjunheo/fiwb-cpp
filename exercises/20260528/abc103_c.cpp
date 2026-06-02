#include <iostream>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  int total = 0;
  for (int i = 0; i < N; i++) {
    int a;
    std::cin >> a;
    total += a - 1;
  }
  std::cout << total << '\n';
}
