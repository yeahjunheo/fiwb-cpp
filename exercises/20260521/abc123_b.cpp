#include <algorithm>
#include <climits>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int minLeft = INT_MAX;
  int total = 0;
  for (int i = 0; i < 5; i++) {
    int x;
    std::cin >> x;
    int left = x % 10;
    if (left > 0) {
      total += x + 10 - left;
    } else {
      total += x;
      left = 10;
    }
    minLeft = std::min(minLeft, left);
  }
  total += minLeft - 10;
  std::cout << total;
}
