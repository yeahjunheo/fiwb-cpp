#include <algorithm>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N, M;
  std::cin >> N >> M;
  int validL = 1, validR = N;
  while (M > 0) {
    M--;
    int L, R;
    std::cin >> L >> R;
    validL = std::max(validL, L);
    validR = std::min(validR, R);
  }
  if (validR >= validL)
    std::cout << validR - validL + 1;
  else
    std::cout << 0;
}
