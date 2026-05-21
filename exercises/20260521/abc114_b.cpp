#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <string>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  const int sevenFiveThree = 753;
  std::string S;
  std::cin >> S;
  int minDiff = INT_MAX;
  for (int i = 0; i < S.length() - 2; i++) {
    int check = std::stoi(S.substr(i, 3));
    minDiff = std::min(minDiff, abs(check - sevenFiveThree));
  }
  std::cout << minDiff;
}
