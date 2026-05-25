#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N, L;
  std::cin >> N >> L;
  std::vector<std::string> pool(N);
  for (int i = 0; i < N; i++) {
    std::cin >> pool[i];
  }
  std::sort(pool.begin(), pool.end());
  for (std::string& word : pool) {
    std::cout << word;
  }
  std::cout << '\n';
}
