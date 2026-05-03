#include <ios>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  vector<int> A(N);
  int count = 0;
  for (int i = 0; i < N; i++) {
    cin >> A[i];
  }
  while (true) {
    for (int& a : A) {
      if (a % 2 == 1) {
        cout << count;
        return 0;
      }
      a /= 2;
    }
    count++;
  }
  return 0;
}