#include <iostream>
bool isPrime(int num) {
  if (num == 2)
    return true;

  if ((num & 1) == 0) {
    return false;
  }

  for (int i = 3; i * i <= num; i++) {
    if (num % i == 0) {
      return false;
    }
  }

  return true;
}
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int X;
  std::cin >> X;
  int limit = X + 200;
  while (X < limit) {
    if (isPrime(X))
      break;
    X += ((X & 1) == 0) ? 1 : 2;
  }
  std::cout << X << "\n";
}
