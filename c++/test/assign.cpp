#include <iostream>
#include <array>

int main()
{
  std::array<int, 1> arr1;
  std::array<int, 1> arr2;

  arr1[0] = 5;
  auto& a1 = arr1[0];
  auto& a2 = arr2[0] = a1;
  a2 = 10;

  std::cout << arr1[0] << '\n';
  std::cout << arr2[0] << '\n';
}

