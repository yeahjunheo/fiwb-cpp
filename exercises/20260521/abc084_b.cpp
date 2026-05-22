#include <iostream>
#include <string>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int A, B;
  std::cin >> A >> B;
  std::string S;
  std::cin >> S;
  for (int i = 0; i < A + B + 1; i++) {
    bool isHyphen = (S[i] == '-');
    bool posHyphen = (i == A);
    if (isHyphen != posHyphen) {
      std::cout << "No";
      return 0;
    }
  }
  std::cout << "Yes";
}
