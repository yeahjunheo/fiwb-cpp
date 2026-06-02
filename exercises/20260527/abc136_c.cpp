#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  int prev = -1;
  for (int i = 0; i < N; i++) {
    int h;
    std::cin >> h;
    if (h - 1 >= prev) {
      prev = h - 1;
    } else if (h >= prev) {
      prev = h;
    } else {
      std::cout << "No" << '\n';
      return 0;
    }
  }
  std::cout << "Yes" << '\n';
}
