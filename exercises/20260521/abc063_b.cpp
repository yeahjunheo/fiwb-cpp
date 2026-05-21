#include <bitset>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::bitset<26> S;
  char s;
  while (std::cin >> s) {
    if (S[s - 'a'] == 1) {
      std::cout << "no";
      return 0;
    }
    S[s - 'a'] = 1;
  }
  std::cout << "yes";
}
