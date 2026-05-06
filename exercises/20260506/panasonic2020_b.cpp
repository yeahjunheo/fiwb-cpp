#include <ios>
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

  const long long H = read<int>();
  const long long W = read<int>();

  long long total = 0;
  if (H == 1 || W == 1) {
    total = 1;
  } else {
    total = (W * H + 1) / 2;
  }

  std::cout << total << "\n";
  return 0;
}