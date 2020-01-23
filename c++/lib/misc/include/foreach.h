#pragma once
#include <functional>

template <typename First, typename... Rest>
void ForEach(std::function<void(First)> func, First first, Rest... rest)
{
    func(first);
    ForEach(rest...);
}

