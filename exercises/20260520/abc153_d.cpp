#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  long long H;
  std::cin >> H;
  long long hitCount = 0;
  long long monsterCount = 1;
  while (H > 1) {
    hitCount += monsterCount;
    monsterCount *= 2;
    H /= 2;
  }
  hitCount += monsterCount;
  std::cout << hitCount;
}
