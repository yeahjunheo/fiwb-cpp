#include <iostream>
#include <climits>
#include <cstdint>

int main() {
    std::cout << "sizeof(char): " << sizeof(char) << "\n";
    std::cout << "sizeof(short): " << sizeof(short) << "\n";
    std::cout << "sizeof(int): " << sizeof(int) << "\n";
    std::cout << "sizeof(long): " << sizeof(long) << "\n";
    std::cout << "sizeof(long long): " << sizeof(long long) << "\n";
    std::cout << "sizeof(float): " << sizeof(float) << "\n";
    std::cout << "sizeof(double): " << sizeof(double) << "\n";
    std::cout << "sizeof(bool): " << sizeof(bool) << "\n";

    std::cout << "\nint goes from " << INT_MIN << " to " << INT_MAX << "\n";

    int32_t a = 100;
    uint64_t b = 100;
    std::cout << "int32_t a = " << a << ", uint64_t b = " << b << "\n";

    return 0;
}