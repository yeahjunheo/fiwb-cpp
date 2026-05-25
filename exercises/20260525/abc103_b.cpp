#include <cstddef>
#include <iostream>
#include <string>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string S, T;
  std::cin >> S >> T;
  for (size_t i = 0; i < S.length(); i++) {
    if (S != T) {
      char frontS = S.back();
      S.pop_back();
      S = frontS + S;
    } else {
      std::cout << "Yes" << '\n';
      return 0;
    }
  }
  std::cout << "No" << '\n';
}
