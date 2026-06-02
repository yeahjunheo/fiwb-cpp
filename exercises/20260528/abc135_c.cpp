#include <algorithm>
#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  std::vector<long long> A(N + 2), B(N + 1);
  for (int i = 0; i < N + 1; i++)
    std::cin >> A[i];
  for (int i = 0; i < N; i++)
    std::cin >> B[i];
  long long deadMonster = 0;
  for (int i = 0; i < N; i++) {
    long long currKill = std::min(B[i], A[i]);
    deadMonster += currKill;
    long long nextKill = std::min(B[i] - currKill, A[i + 1]);
    deadMonster += nextKill;
    A[i + 1] -= nextKill;
  }
  std::cout << deadMonster << '\n';
}
