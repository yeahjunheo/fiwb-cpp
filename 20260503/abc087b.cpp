#include <ios>
#include <iostream>
using namespace std;

int read_int() {
  int x;
  cin >> x;
  return x;
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  const int A = read_int();  // 500-yen
  const int B = read_int();  // 100-yen
  const int C = read_int();  // 50-yen
  const int X = read_int();

  int count = 0;

  for (int a = 0; a <= A; a++) {
    for (int b = 0; b <= B; b++) {
      for (int c = 0; c <= C; c++) {
        count += 1 ? 500 * a + 100 * b + 50 * c == X : 0;
      }
    }
  }

  cout << count;
  return 0;
}
