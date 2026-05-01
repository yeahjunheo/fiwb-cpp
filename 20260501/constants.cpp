#include <cstdint>
#include <iostream>
int main() {
  constexpr int32_t MAX_ORDER_QTY = 10000;
  constexpr double TICK_SIZE = 0.01;
  constexpr int64_t NS_PER_SECOND = 1'000'000'000LL;
  constexpr int32_t TIMESTAMP_BYTES = sizeof(int64_t);

  std::cout << "Max order qty: " << MAX_ORDER_QTY << "\n";
  std::cout << "Tick size: " << TICK_SIZE << "\n";
  std::cout << "Nanoseconds/sec: " << NS_PER_SECOND << "\n";
  std::cout << "Timestamp bytes: " << TIMESTAMP_BYTES << "\n";

  int32_t base_qty = 200;
  base_qty = base_qty * 3;
  const int32_t session_cap = base_qty;
  std::cout << "Session cap: " << session_cap << "\n";

  // TICK_SIZE = 0.05;

  return 0;
}