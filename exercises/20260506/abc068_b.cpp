#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  int p = 1;
  while (p * 2 <= N)
    p *= 2;
  std::cout << p << "\n";

  return 0;
}