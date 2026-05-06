#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int a, b;
  std::cin >> a >> b;
  int p = 1;
  int t = b;
  while (t > 0) {
    p *= 10;
    t /= 10;
  }
  int AB = a * p + b;

  long long ab = std::sqrt(AB);
  bool perfect = (ab * ab == AB);
  std::cout << (perfect ? "Yes\n" : "No\n");
  return 0;
}