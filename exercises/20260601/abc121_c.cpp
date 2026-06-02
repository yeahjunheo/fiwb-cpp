#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N, M;
  std::cin >> N >> M;
  std::vector<std::pair<int, int>> shops(N);
  for (auto& [a, b] : shops) std::cin >> a >> b;
  std::sort(shops.begin(), shops.end());

  long long totalCost = 0, totalCans = 0;
  for (const auto& [price, stock] : shops) {
    if (totalCans >= M) break;
    long long cans = std::min<long long>(stock, M - totalCans);
    totalCost += (long long)price * cans;
    totalCans += cans;
  }
  std::cout << totalCost << '\n';
}
