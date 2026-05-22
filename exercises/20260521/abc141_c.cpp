#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  long long N, K, Q;
  std::cin >> N >> K >> Q;
  std::vector<int> playerAnswered(N, 0);
  for (int i = 0; i < Q; i++) {
    int A;
    std::cin >> A;
    playerAnswered[A - 1]++;
  }
  for (int score : playerAnswered) {
    if (K - Q + score > 0) {
      std::cout << "Yes\n";
    } else {
      std::cout << "No\n";
    }
  }
}
