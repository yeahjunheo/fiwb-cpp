#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  vector<int> A(N);
  for (int i = 0; i < N; i++) {
    cin >> A[i];
  }
  //   sort(A.begin(), A.end());
  //   int res = 0;
  //   while (A.size() > 1) {
  //     res += A.back();
  //     A.pop_back();
  //     res -= A.back();
  //     A.pop_back();
  //   }

  sort(A.begin(), A.end(), greater<>());
  long long res = 0;
  for (int i = 0; i < N; ++i) {
    res += (i % 2 == 0 ? A[i] : -A[i]);
  }
  if (A.size() == 1)
    res += A.back();
  cout << res << "\n";
  return 0;
}