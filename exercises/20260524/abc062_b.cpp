#include <iostream>
#include <string>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int H, W;
  std::cin >> H >> W;
  std::string border = std::string((W + 2), '#');
  std::cout << border << '\n';
  for (int i = 0; i < H; i++) {
    std::string row;
    std::cin >> row;
    std::cout << '#' << row << '#' << '\n';
  }
  std::cout << border << '\n';
}
