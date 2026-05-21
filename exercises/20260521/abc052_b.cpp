#include <algorithm>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int x = 0;
  int N;
  std::cin >> N;
  int maxX = x;
  while (N > 0) {
    N--;
    char s;
    std::cin >> s;
    if (s == 'I') {
      x++;
    } else {
      x--;
    }
    maxX = std::max(maxX, x);
  }
  std::cout << maxX;
}
