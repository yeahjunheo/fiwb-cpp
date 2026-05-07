#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  long long a, b, c;
  std::cin >> a >> b >> c;
  int count = 0;
  while (((a | b | c) & 1) == 0) {
    if (a == b && b == c) {
      count = -1;
      break;
    }
    long long tempA = b / 2 + c / 2, tempB = a / 2 + c / 2, tempC = a / 2 + b / 2;
    a = tempA;
    b = tempB;
    c = tempC;
    count++;
  }
  std::cout << count << "\n";
}