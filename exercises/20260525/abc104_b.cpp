#include <cstddef>
#include <iostream>
#include <string>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string S;
  std::cin >> S;
  bool sawC = false;
  for (size_t i = 0; i < S.length(); i++) {
    if (i == 0 && S[i] != 'A') {
      std::cout << "WA" << '\n';
      return 0;
    }
    if (S[i] == 'C') {
      if (i < 2 || i > S.length() - 2) {
        std::cout << "WA" << '\n';
        return 0;
      }
      if (sawC) {
        std::cout << "WA" << '\n';
        return 0;
      }
      sawC = true;
    }
    if (i != 0 && S[i] != 'C') {
      if (S[i] >= 'A' && S[i] <= 'Z') {
        std::cout << "WA" << '\n';
        return 0;
      }
    }
  }
  std::cout << ((sawC) ? "AC" : "WA") << '\n';
}
