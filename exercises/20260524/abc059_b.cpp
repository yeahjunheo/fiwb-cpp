#include <iostream>
#include <string>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string A, B;
  std::cin >> A >> B;
  if (A.length() != B.length()) {
    std::cout << ((A.length() > B.length()) ? "GREATER\n" : "LESS\n");
  } else if (A > B) {
    std::cout << "GREATER\n";
  } else if (A < B) {
    std::cout << "LESS\n";
  } else {
    std::cout << "EQUAL\n";
  }
}
