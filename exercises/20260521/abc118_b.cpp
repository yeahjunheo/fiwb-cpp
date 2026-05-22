#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N, M;
  std::cin >> N >> M;
  std::vector<int> likedByAll(M, 0);
  for (int i = 0; i < N; i++) {
    int K;
    std::cin >> K;
    for (int j = 0; j < K; j++) {
      int food;
      std::cin >> food;
      likedByAll[food - 1]++;
    }
  }
  int count = 0;
  for (int i = 0; i < M; i++) {
    if (likedByAll[i] == N) {
      count++;
    }
  }
  std::cout << count << "\n";
}
