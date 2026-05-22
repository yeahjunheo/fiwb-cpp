#include <iostream>
#include <string>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int A, B;
  std::cin >> A >> B;
  int count = 0;
  for (int i = A; i <= B; i++) {
    std::string s = std::to_string(i);
    if (s[0] == s[4] && s[1] == s[3])
      count++;
  }
  std::cout << count << "\n";
}
