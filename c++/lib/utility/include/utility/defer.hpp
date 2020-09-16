#pragma once
#include <functional>

#define DEFER_JOIN(x, y) x##y

#define defer Defer DEFER_JOIN(defer_, __COUNTER__)

class Defer
{
 public:
  Defer(std::function<void(void)> f);
  ~Defer();

 private:
  std::function<void(void)> mFunc;
};

inline Defer::Defer(std::function<void(void)> f): mFunc(f) {}

inline Defer::~Defer()
{
  mFunc();
}