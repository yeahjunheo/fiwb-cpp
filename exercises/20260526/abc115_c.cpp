#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N, K;
  std::cin >> N >> K;
  std::vector<int> H(N);
  for (int i = 0; i < N; i++) {
    std::cin >> H[i];
  }
  std::sort(H.begin(), H.end());
  int heightGap = INT_MAX;
  int right = K - 1;
  for (int i = 0; right < N; i++) {
    heightGap = std::min(heightGap, H[right] - H[i]);
    right++;
  }
  std::cout << heightGap << '\n';
}
