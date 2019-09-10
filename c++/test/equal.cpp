#include <iostream>
#include <algorithm>
#include <stdlib.h>

int main() {
    auto str1 = new char[6];
    auto str2 = new char[6];
    memcpy(str1, "hello", sizeof(char) * 6);
    memcpy(str2, "hello", sizeof(char) * 6);

    if (std::equal(str1, str2)) {
	std::cout << "Equal" << '\n';
    } else {
	std::cout << "Not Equal" << '\n';
    }

    delete[] str1;
    delete[] str2;
    return 0;
}

