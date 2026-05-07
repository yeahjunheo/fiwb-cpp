#include <iostream>
int step(int num) {
  return (num & 1) ? 3 * num + 1 : num / 2;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int s;
  std::cin >> s;
  int slow = s, fast = s;
  do {
    slow = step(slow);
    fast = step(step(fast));
  } while (slow != fast);

  slow = s;
  int T = 0;
  while (slow != fast) {
    slow = step(slow);
    fast = step(fast);
    T++;
  }

  int C = 1;
  int cur = step(slow);
  while (cur != slow) {
    cur = step(cur);
    C++;
  }

  std::cout << T + C + 1 << '\n';
}

// #include <bitset>
// #include <iostream>
// int main() {
//   std::ios_base::sync_with_stdio(false);
//   std::cin.tie(nullptr);

//   int s;
//   std::cin >> s;
//   int count = 1;
//   std::bitset<1'000'001> seen;
//   while (!seen.test(s)) {
//     seen.set(s);
//     s = (s & 1) ? 3 * s + 1 : s / 2;
//     count++;
//   }
//   std::cout << count << '\n';
// }
