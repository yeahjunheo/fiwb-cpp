#include <algorithm>
#include <climits>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  // int opCount = INT_MAX;
  // while (N > 0) {
  //   N--;
  //   int A;
  //   std::cin >> A;
  //   int count = 0;
  //   while (A % 2 == 0) {
  //     A /= 2;
  //     count++;
  //   }
  //   opCount = std::min(opCount, count);
  // }
  // std::cout << opCount;
  unsigned acc = 0;
  while (N > 0) {
    N--;
    int A;
    std::cin >> A;
    acc |= static_cast<unsigned>(A);
  }
  std::cout << std::__countr_zero(acc);
}
