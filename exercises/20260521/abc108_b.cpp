#include <iostream>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int x1, y1, x2, y2;
  std::cin >> x1 >> y1 >> x2 >> y2;
  int xDiff = x2 - x1;
  int yDiff = y2 - y1;
  int x3, y3, x4, y4;
  x3 = x2 - yDiff;
  y3 = y2 + xDiff;
  x4 = x1 - yDiff;
  y4 = y1 + xDiff;
  std::cout << x3 << " " << y3 << " " << x4 << " " << y4;
}
