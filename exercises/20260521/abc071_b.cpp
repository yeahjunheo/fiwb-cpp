#include <bitset>
#include <iostream>
#include <string>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::string S;
  std::cin >> S;
  std::bitset<26> alphacheck;
  for (char s : S) {
    alphacheck[s - 'a'] = 1;
  }
  for (int i = 0; i < 26; i++) {
    if (alphacheck[i] != 1) {
      std::cout << static_cast<char>(i + 'a');
      return 0;
    }
  }
  std::cout << "None";
}
