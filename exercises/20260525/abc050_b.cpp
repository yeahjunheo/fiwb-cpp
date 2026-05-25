#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  int totalTime = 0;
  std::vector<int> problemTime(N);
  for (int i = 0; i < N; i++) {
    std::cin >> problemTime[i];
    totalTime += problemTime[i];
  }
  int M;
  std::cin >> M;
  for (int i = 0; i < M; i++) {
    int p, x;
    std::cin >> p >> x;
    std::cout << totalTime - problemTime[p - 1] + x << '\n';
  }
}
