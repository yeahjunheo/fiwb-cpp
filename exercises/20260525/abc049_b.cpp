#include <iostream>
#include <string>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int H, W;
  std::cin >> H >> W;
  for (int i = 0; i < H; i++) {
    std::string printLine;
    std::cin >> printLine;
    std::cout << printLine << '\n' << printLine << '\n';
  }
}
