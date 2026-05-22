#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  long long A, B, C, K;
  std::cin >> A >> B >> C >> K;
  std::cout << ((K % 2 == 0) ? A - B : B - A) << "\n";
}
