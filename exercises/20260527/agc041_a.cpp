#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  long long N, A, B;
  std::cin >> N >> A >> B;
  long long games = 0;
  if ((B - A) % 2 == 0) {
    games = (B - A) / 2;
  } else {
    if (A <= N - B) {
      games += A - 1;
      games += (B - A - 1) / 2;
    } else {
      games += N - B;
      games += (B - A - 1) / 2;
    }
    games++;
  }
  std::cout << games << '\n';
}
