#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  long long prevL = 2, currL = 1;
  for (int i = 2; i <= N; i++) {
    long long temp = currL;
    currL += prevL;
    prevL = temp;
  }
  std::cout << currL << "\n";
}
