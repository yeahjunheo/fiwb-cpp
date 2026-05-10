#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int A, B, M;
  std::cin >> A >> B >> M;
  std::vector<int> fridge(A), microwave(B);
  int minFridge = INT_MAX, minMicrowave = INT_MAX;
  for (int i = 0; i < A; i++) {
    std::cin >> fridge[i];
    minFridge = std::min(minFridge, fridge[i]);
  }
  for (int i = 0; i < B; i++) {
    std::cin >> microwave[i];
    minMicrowave = std::min(minMicrowave, microwave[i]);
  }
  int minAmount = minFridge + minMicrowave;
  for (int i = 0; i < M; i++) {
    int x, y, c;
    std::cin >> x >> y >> c;
    int price = fridge[x - 1] + microwave[y - 1] - c;
    minAmount = std::min(minAmount, price);
  }
  std::cout << minAmount << "\n";
}
