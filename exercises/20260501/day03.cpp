#include <cstdint>
#include <iostream>
int main() {
  constexpr double TICK = 0.01;
  constexpr int64_t NS_PER_MS = 1'000'000LL;
  //   const double spread = 2.0 * TICK;
  int32_t qty = 200;
  int32_t runtime_val = qty;
  const int32_t locked = runtime_val;

  std::cout << TICK << "\n";
  std::cout << NS_PER_MS << "\n";
  //   std::cout << spread << "\n";
  std::cout << qty << "\n";
  std::cout << locked << "\n";
  locked = locked + 1;

  qty = qty * 3;
  std::cout << "qty is the only one that is mutable: " << qty << "\n";

  //   TICK = 0.5;

  return 0;
}