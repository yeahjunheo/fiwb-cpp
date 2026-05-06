#include <climits>
#include <cstdint>
#include <iostream>
int main() {
    int64_t x = INT64_MAX;
    std::cout << "x = " << x << "\n";
    std::cout << "x + 1 = " << x + 1 << "\n";

    unsigned int u = 0;
    std::cout << "u = " << u << "\n";
    std::cout << "u - 1 = " << u - 1 << "\n";

    return 0;
}