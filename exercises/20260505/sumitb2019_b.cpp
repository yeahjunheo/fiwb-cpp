#include <ios>
#include <iostream>

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;

  int X = ((N * 100) / 108 + 107) / 108;
  if ((X + 107) / 108 == N)
    std::cout << X << "\n";
  else
    std::cout << ":(";
  return 0;
}