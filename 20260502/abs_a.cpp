#include <iostream>
#include <string>
int main() {
  int a, b, c;
  std::string s;
  std::cin >> a;
  std::cin >> b >> c;
  std::cin >> s;
  a = a + b + c;
  std::cout << a << " " << s;
}