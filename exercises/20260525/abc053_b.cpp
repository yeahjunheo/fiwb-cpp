#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int idxA = -1, idxZ = -1;
  char s;
  int currIdx = 0;
  while (std::cin >> s) {
    if (s == 'A' && idxA == -1) {
      idxA = currIdx;
    } else if (s == 'Z') {
      idxZ = currIdx;
    }
    currIdx++;
  }
  std::cout << ((idxA <= idxZ && idxA != -1 && idxZ != -1) ? idxZ - idxA + 1 : -1);
}
