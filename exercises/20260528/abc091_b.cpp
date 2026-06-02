#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  std::unordered_map<std::string, int> cardCounter;
  for (int i = 0; i < N; i++) {
    std::string S;
    std::cin >> S;
    cardCounter[S]++;
  }
  int M;
  std::cin >> M;
  for (int i = 0; i < M; i++) {
    std::string T;
    std::cin >> T;
    cardCounter[T]--;
  }
  int res = 0;
  for (const auto& [key, value] : cardCounter) {
    res = std::max(res, value);
  }
  std::cout << res << '\n';
}
