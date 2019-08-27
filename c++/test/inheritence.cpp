#include <iostream>

enum class Diff {
    AClass,
    BClass
};

template <typename T, Diff D>
class A {
    public:
	const char* toString() {
	    return "A";
	}
};

template <typename T>
class A<T, Diff::BClass> : public A<int, Diff::AClass> {
    public:
	const char* toString() {
	    return "B";
	}
};

int main() {
    A<const char*, Diff::BClass> b; 
    std::cout << b.toString() << '\n';
    return 0;
}

