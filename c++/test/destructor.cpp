#include <iostream>
#include <functional>

class Foo
{
 public:
  Foo(bool thing): mThing(thing) {}
  ~Foo()
  {
    if (mFunc) {
      mFunc();
    }

    if (!mThing) {
      std::cout << "not thing\n";
    }
  }

  void doThing(std::function<void(void)> func)
  {
    mFunc = func;
  }

 private:
  bool mThing;
  std::function<void(void)> mFunc;
};

Foo mkfoo(bool thing) {
	return Foo(thing);
}

// before
// in thing
// not thing
// after

int main()
{
	std::cout << "before\n";
  mkfoo(false).doThing([] {
    std::cout << "in thing\n";
  });

	std::cout << "after\n";
  return 0;
}
