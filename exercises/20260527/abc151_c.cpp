#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N, M;
  std::cin >> N >> M;
  std::unordered_set<int> seenAC;
  std::unordered_map<int, int> pending;
  int penalty = 0;
  for (int i = 0; i < M; i++) {
    int p;
    std::string status;
    std::cin >> p >> status;
    if (status == "AC") {
      if (seenAC.insert(p).second) {
        penalty += pending[p];
        pending.erase(p);
      }
    } else if (!seenAC.count(p)) {
      pending[p]++;
    }
  }
  std::cout << seenAC.size() << ' ' << penalty << '\n';
}
