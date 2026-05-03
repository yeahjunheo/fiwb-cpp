#include <iostream>
#include <unordered_set>
using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  unordered_set<int> D;
  for (int i = 0; i < N; i++) {
    int d;
    cin >> d;
    D.insert(d);
  }
  cout << D.size() << "\n";
  return 0;
}