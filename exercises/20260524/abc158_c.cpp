#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int A, B;
  std::cin >> A >> B;
  int startVal = (A * 100 + 7) / 8;
  int endVal = (B + 1) * 10 - 1;
  for (int i = startVal; i <= endVal; i++) {
    int tax8 = i * 8 / 100;
    int tax10 = i * 10 / 100;
    if (tax8 == A && tax10 == B) {
      std::cout << i << '\n';
      return 0;
    }
  }
  std::cout << -1 << '\n';
}
