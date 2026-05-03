#include <iostream>
#include <string>
using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  string s;
  cin >> s;

  int count = 0;
  for (char c : s) {
    if (c == '1')
      ++count;
  }

  cout << count << "\n";
  return 0;
}
