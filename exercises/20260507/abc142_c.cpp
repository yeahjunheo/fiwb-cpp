#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  std::vector<int> A(N);
  for (int i = 0; i < N; i++)
    std::cin >> A[i];
  std::vector<int> res(N);
  for (int i = 0; i < N; i++) {
    res[A[i] - 1] = i + 1;
  }
  for (int i = 0; i < N; i++)
    std::cout << res[i] << (i == N - 1 ? "\n" : " ");
}
