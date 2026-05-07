#include <algorithm>
#include <iostream>
#include <string>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string S;
  std::cin >> S;
  std::string acgt = "ACGT";
  int count = 0;
  int res = 0;
  for (char s : S) {
    if (acgt.find(s) != std::string::npos) {
      count++;
      res = std::max(res, count);
    } else {
      count = 0;
    }
  }

  std::cout << res << '\n';
}
