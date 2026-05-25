#include <iostream>
#include <string>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  std::string S, prevSeen = "", subS = "";
  std::cin >> S;
  int count = 0;
  for (char& s : S) {
    subS += s;
    if (subS != prevSeen) {
      prevSeen = subS;
      subS.clear();
      count++;
    }
  }
  std::cout << count << '\n';
}
