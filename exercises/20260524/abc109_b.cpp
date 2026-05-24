#include <iostream>
#include <string>
#include <unordered_set>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  std::string prevW;
  std::cin >> prevW;
  std::unordered_set<std::string> seen = {prevW};
  for (int i = 1; i < N; i++) {
    std::string W;
    std::cin >> W;
    if ((W.front() != prevW.back()) || !seen.insert(W).second) {
      std::cout << "No\n";
      return 0;
    }
    prevW = W;
  }
  std::cout << "Yes\n";
}
