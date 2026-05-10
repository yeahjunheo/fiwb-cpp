#include <algorithm>
#include <iostream>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  long long x;
  std::cin >> N >> x;
  std::vector<long long> A(N);
  for (int i = 0; i < N; i++) {
    std::cin >> A[i];
  }
  std::sort(A.begin(), A.end());
  int count = 0;
  for (int i = 0; i < N; i++) {
    if (x >= A[i]) {
      x -= A[i];
      count++;
    } else {
      break;
    }
  }
  if (count == N && x > 0) {
    count--;
  }
  std::cout << count << "\n";
}