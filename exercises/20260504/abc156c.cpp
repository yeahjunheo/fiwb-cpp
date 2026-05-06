#include <climits>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  vector<int> pos(N);
  int max_pos = 1;
  for (int i = 0; i < N; i++) {
    cin >> pos[i];
    if (pos[i] > max_pos)
      max_pos = pos[i];
  }
  long long res = LLONG_MAX;
  for (int x = 1; x <= max_pos; x++) {
    long long stamina = 0;
    for (int p : pos) {
      stamina += (long long)(p - x) * (p - x);
    }
    if (res > stamina)
      res = stamina;
  }

  cout << res << "\n";
  return 0;
}