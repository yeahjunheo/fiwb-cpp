#include <iostream>
#include <map>
#include <string>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  std::map<std::string, int> stringCounter;
  int maxCount = 0;
  for (int i = 0; i < N; i++) {
    std::string S;
    std::cin >> S;
    int c = ++stringCounter[S];
    if (c > maxCount)
      maxCount = c;
  }
  for (const auto& [key, value] : stringCounter) {
    if (value == maxCount) {
      std::cout << key << '\n';
    }
  }
}
