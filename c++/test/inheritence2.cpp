#include <iostream>

// visitor
template <typename T>
class Foobar {
   public:
    virtual T convert() = 0;

   protected:
    Foobar() = default;
};

// extends visitor
class Foo : public Foobar<const char*> {
    public:
	const char* convert() override { return "Foo"; }
};

// extends visitor
class Bar : public Foobar<std::string> {
    public:
	std::string convert() override { return "Bar"; }
};

// expr
template <typename T>
class Anon {
   public:
    virtual T handle(Foobar<T>& thing)=0;
};

// extends expr
template <typename T>
class Named : public Anon<T> {
   public:
    T handle(Foobar<T>& thing) { return thing.convert(); }
};

int main() {
    Foo foo;
    Bar bar;
    Named<std::string> named;
    Anon<std::string>& anon = named;

    //std::cout << anon.handle(foo) << '\n';
    std::cout << anon.handle(bar) << '\n';
    return 0;
}

