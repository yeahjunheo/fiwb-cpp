#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N, M, C;
  std::cin >> N >> M >> C;
  std::vector<int> B(M);
  for (int i = 0; i < M; i++) {
    std::cin >> B[i];
  }

  int count = 0;
  for (int _ = 0; _ < N; _++) {
    int temp = C;
    for (int i = 0; i < M; i++) {
      int A;
      std::cin >> A;
      temp += A * B[i];
    }
    if (temp > 0)
      count++;
  }

  std::cout << count << "\n";
  return 0;
}