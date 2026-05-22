#include <algorithm>
#include <climits>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  int count = 0;
  int currentMin = INT_MAX;
  for (int i = 0; i < N; i++) {
    int p;
    std::cin >> p;
    if (p <= currentMin) {
      count++;
      currentMin = std::min(currentMin, p);
    }
  }
  std::cout << count << "\n";
}
