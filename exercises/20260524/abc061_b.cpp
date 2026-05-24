#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N, M;
  std::cin >> N >> M;
  std::vector<int> roads(N + 1);
  for (int i = 0; i < M; i++) {
    int a, b;
    std::cin >> a >> b;
    roads[a]++;
    roads[b]++;
  }
  for (int i = 1; i < N + 1; i++) {
    std::cout << roads[i] << '\n';
  }
}
