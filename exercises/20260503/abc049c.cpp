#include <array>
#include <cstddef>
#include <iostream>
#include <string_view>
using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  string S;
  cin >> S;
  constexpr array<string_view, 4> words = {"dreamer"sv, "eraser"sv, "dream"sv, "erase"sv};
  size_t end = S.size();
  while (end > 0) {
    bool matched = false;
    for (auto w : words) {
      if (end >= w.size() && S.compare(end - w.size(), w.size(), w) == 0) {
        end -= w.size();
        matched = true;
        break;
      }
    }
    if (!matched) {
      cout << "NO\n";
      return 0;
    }
  }
  cout << "YES\n";
  return 0;
}