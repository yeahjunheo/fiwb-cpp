#include <iostream>

int sum_of_digits(int num) {
  int sum = 0;
  while (num > 0) {
    sum += num % 10;
    num /= 10;
  }
  return sum;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N, A, B;
  std::cin >> N >> A >> B;
  int sum = 0;
  for (int i = 1; i <= N; i++) {
    int s = sum_of_digits(i);
    if (A <= s && s <= B) {
      sum += i;
    }
  }
  std::cout << sum << '\n';
}
