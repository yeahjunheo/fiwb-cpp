#include <cstdint>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  uint32_t mask = 0;
  char w;
  while (std::cin >> w) {
    mask ^= (1u << (w - 'a'));
  }
  std::cout << (mask == 0 ? "Yes" : "No") << "\n";
}
