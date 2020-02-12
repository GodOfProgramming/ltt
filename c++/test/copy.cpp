#include <array>
#include <algorithm>
#include <cinttypes>
#include <iostream>

int main()
{
    std::array<uint16_t, 8> arr8;
    std::array<uint8_t, 16> arr16;

    for (uint8_t i = 0; i < arr8.size(); i++) {
        arr8[i] = i;
    }

    std::copy(arr8.begin(), arr8.data(), arr16.begin());

    for (auto& i : arr16) {
        std::cout << (int)i << '\n';
    }
    return 0;
}
