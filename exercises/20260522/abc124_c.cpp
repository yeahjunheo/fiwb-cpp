#include <algorithm>
#include <iostream>
#include <string>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string S;
  std::cin >> S;
  int count = 0;
  for (int i = 0; i < (int)S.length(); i++) {
    if (S[i] != '0' + (i % 2)) {
      count++;
    }
  }
  std::cout << std::min(count, (int)S.length() - count) << "\n";
}
