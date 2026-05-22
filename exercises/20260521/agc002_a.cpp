#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int a, b;
  std::cin >> a >> b;
  if (b < 0) {
    if ((b - a + 1) % 2 == 0) {
      std::cout << "Positive\n";
    } else {
      std::cout << "Negative\n";
    }
  } else if (a > 0) {
    std::cout << "Positive\n";
  } else {
    std::cout << "Zero\n";
  }
}
