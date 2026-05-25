#include <cstddef>
#include <iostream>
#include <string>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string O, E;
  std::cin >> O >> E;
  int oIdx = 0, eIdx = 0;
  std::string password = "";
  for (size_t i = 0; i < O.size() + E.size(); i++) {
    if (i % 2 == 0) {
      password += O[oIdx];
      oIdx++;
    } else {
      password += E[eIdx];
      eIdx++;
    }
  }
  std::cout << password << '\n';
}
