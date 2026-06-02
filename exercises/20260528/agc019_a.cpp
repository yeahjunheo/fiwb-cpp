#include <algorithm>
#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  long long Q, H, S, D;
  std::cin >> Q >> H >> S >> D;
  long long minLitre = std::min(Q * 4, H * 2);
  minLitre = std::min(minLitre, S);
  long long N;
  std::cin >> N;
  long long bestDeal;
  if (minLitre * 2 > D) {
    bestDeal = N / 2 * D + N % 2 * minLitre;
  } else {
    bestDeal = N * minLitre;
  }
  std::cout << bestDeal << '\n';
}
