#include <iostream>
using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Y;
  cin >> N >> Y;

  // m + g + s = N, 10000m + 5000g + 1000s = Y
  // → 4000g = Y - 1000N - 9000m
  for (int m = 0; m <= N; m++) {
    int rem = Y - 1000 * N - 9000 * m;
    if (rem < 0)
      break;
    if (rem % 4000 != 0)
      continue;
    int g = rem / 4000;
    int s = N - m - g;
    if (g >= 0 && s >= 0) {
      cout << m << " " << g << " " << s << "\n";
      return 0;
    }
  }

  cout << "-1 -1 -1\n";
  return 0;
}