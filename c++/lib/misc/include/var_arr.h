#pragma once
#include <vector>

template <unsigned int Size, typename T, typename... Ts>
class VarArr
{
   public:
    constexpr VarArr(Ts... Args) : mArr({ Args... })
    {}

    inline operator std::vector<T>()
    {
        return mArr;
    }

   private:
    const std::vector<T> mArr;
};

