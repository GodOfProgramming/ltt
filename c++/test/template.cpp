class Foo
{
 public:
  template <typename T>
  auto Test(T& t) -> void
  {
    t.template Test<T>(*this);
  }
};

int main()
{
  Foo foo, bar;
  foo.Test(bar);
  return 0;
}
