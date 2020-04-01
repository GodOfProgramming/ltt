#include <map>
#include <string>
#include <iostream>

class map
{
 public:
  void set(std::string str, std::string val)
  {
    mMap[str] = val;
  }

  void operator,(std::string str)
  {
    std::cout << mMap[str] << '\n';
  }

 private:
  std::map<std::string, std::string> mMap;
};

int main()
{
  map m;

	m.set("Str", "str");
  m, "Str";

  return 0;
}
