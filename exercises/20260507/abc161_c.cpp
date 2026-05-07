#include <algorithm>
#include <cstdlib>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  long long N, K;
  std::cin >> N >> K;
  N %= K;
  std::cout << std::min(N, K - N) << "\n";
}
