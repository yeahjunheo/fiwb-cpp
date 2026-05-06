#include <iostream>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int A[3][3];
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      std::cin >> A[i][j];

  int N;
  std::cin >> N;
  bool called[101] = {};
  while (N--) {
    int b;
    std::cin >> b;
    called[b] = true;
  }

  bool marked[3][3] = {};

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      marked[i][j] = called[A[i][j]];

  auto won = [&]() {
    for (int i = 0; i < 3; i++) {
      if (marked[i][0] && marked[i][1] && marked[i][2])
        return true;
      if (marked[0][i] && marked[1][i] && marked[2][i])
        return true;
    }
    if (marked[0][0] && marked[1][1] && marked[2][2])
      return true;
    if (marked[2][0] && marked[1][1] && marked[0][2])
      return true;
    return false;
  };
  std::cout << (won() ? "Yes" : "No") << "\n";

  return 0;
}