#include <iostream>
#include <string>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string S;
  std::cin >> S;
  long long K;
  std::cin >> K;
  for (char num : S) {
    if (num == '1') {
      K -= 1;
      if (K == 0) {
        std::cout << num << '\n';
        return 0;
      }
    } else {
      std::cout << num << '\n';
      return 0;
    }
  }
}
