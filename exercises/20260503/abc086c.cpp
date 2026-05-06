#include <cstdlib>
#include <iostream>
using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  int curr_t = 0, curr_x = 0, curr_y = 0;
  for (int i = 0; i < N; i++) {
    int t, x, y;
    cin >> t >> x >> y;
    int pos_diff = abs(curr_x - x) + abs(curr_y - y);
    int t_diff = t - curr_t;
    if ((t_diff < pos_diff) || ((t_diff - pos_diff) % 2)) {
      cout << "No\n";
      return 0;
    }
    curr_t = t;
    curr_x = x;
    curr_y = y;
  }
  cout << "Yes\n";
  return 0;
}