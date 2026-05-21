#include <algorithm>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  long long N, A, B;
  std::cin >> N >> A >> B;
  // long long count = 0;
  // while (N > A + B) {
  //   count += A;
  //   N -= (A + B);
  // }
  // count += std::min(N, A);
  // std::cout << count;
  long long count = (N / (A + B)) * A;
  N %= (A + B);
  count += std::min(N, A);
  std::cout << count;
}
