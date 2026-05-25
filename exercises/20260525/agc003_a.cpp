#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  bool sawN = false, sawW = false, sawS = false, sawE = false;
  char direction;
  while (std::cin >> direction) {
    if (direction == 'N') {
      sawN = true;
    } else if (direction == 'W') {
      sawW = true;
    } else if (direction == 'S') {
      sawS = true;
    } else {
      sawE = true;
    }
  }
  if (sawS != sawN || sawW != sawE) {
    std::cout << "No" << '\n';
  } else {
    std::cout << "Yes" << '\n';
  }
}
