#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int N;
  std::cin >> N;
  std::vector<std::tuple<std::string, int, int>> record(N);
  for (int i = 0; i < N; i++) {
    std::cin >> std::get<0>(record[i]) >> std::get<1>(record[i]);
    std::get<1>(record[i]) = -std::get<1>(record[i]);
    std::get<2>(record[i]) = i + 1;
  }
  std::sort(record.begin(), record.end());
  for (auto& [_, __, id] : record) {
    std::cout << id << '\n';
  }
}
