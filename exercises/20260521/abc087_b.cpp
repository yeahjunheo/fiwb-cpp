#include <iostream>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int A, B, C, X;
  std::cin >> A >> B >> C >> X;
  int count = 0;
  for (int i = 0; i <= A; i++) {
    for (int j = 0; j <= B; j++) {
      int neededTot = X - (500 * i + 100 * j);
      bool valid = (neededTot >= 0) && (neededTot % 50 == 0) && (neededTot / 50 <= C);
      if (valid)
        count++;
    }
  }
  std::cout << count;
}
