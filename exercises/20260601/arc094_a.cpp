#include <algorithm>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int pool[3];
  for (int i = 0; i < 3; i++) {
    std::cin >> pool[i];
  }
  std::sort(pool, pool + 3);
  int oneSum = pool[2] - pool[1];
  int twoSum = pool[1] - pool[0];
  int ops = 0;
  ops += oneSum;
  if (twoSum % 2 == 0) {
    ops += twoSum / 2;
  } else {
    ops += (twoSum + 1) / 2 + 1;
  }
  std::cout << ops << '\n';
}
