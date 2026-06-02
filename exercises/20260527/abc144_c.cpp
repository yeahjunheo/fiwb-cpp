#include <iostream>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  long long N;
  std::cin >> N;
  long long minSteps = N - 1;
  for (long long i = 1; i * i <= N; i++) {
    if (N % i == 0) {
      minSteps = i + N / i - 2;
    }
  }
  std::cout << minSteps << '\n';
}
