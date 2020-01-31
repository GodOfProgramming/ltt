#include <iostream>

int main() {
#ifdef _GLIBCXX_HAS_GTHREADS
    std::cout << "have them\n";
#else
    std::cout << "does not have them\n";
#endif
    return 0;
}
