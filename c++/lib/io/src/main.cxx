#include <iostream>
#include "io/file.hpp"
#include "io/dir.hpp"

int main()
{
    io::Dir d("test-dir");
    io::File f("test-dir/text.txt");

    std::cout << "d exists: " << d.exists() << '\n';
    std::cout << "f exists: " << f.exists() << '\n';

    return 0;
}

