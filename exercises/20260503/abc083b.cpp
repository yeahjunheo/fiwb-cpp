#include <ios>
#include <iostream>
using namespace std;

int sum_of_digits(int num) {
  int res = 0;
  while (num) {
    res += num % 10;
    num /= 10;
  }
  return res;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, A, B;
  cin >> N >> A >> B;
  int res = 0;
  for (int i = 1; i <= N; i++) {
    int sum = sum_of_digits(i);
    if (sum >= A && sum <= B) {
      res += i;
    }
  }
  cout << res;
  return 0;
}