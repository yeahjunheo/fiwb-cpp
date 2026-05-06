#include <iostream>

template <class T>
T read() {
  T x;
  std::cin >> x;
  return x;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  const int N = read<int>();
  const int A = read<int>();
  const int B = read<int>();
  const auto S = read<std::string>();

  int domesticPassed = 0;
  int foreignPassed = 0;
  for (int i = 0; i < N; i++) {
    bool ok = false;
    if (domesticPassed + foreignPassed < A + B) {
      if (S[i] == 'a') {
        ok = true;
        ++domesticPassed;
      } else if ((S[i] == 'b') && (foreignPassed < B)) {
        ok = true;
        ++foreignPassed;
      }
    }
    std::cout << (ok ? "Yes\n" : "No\n");
  }
  return 0;
}