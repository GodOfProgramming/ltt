#pragma once
#include <type_traits>

/* Declare a function with the specified return type and name.
 * __VAR_ARGS__ may optionally include "T..." (though why use it if you're not going to)
 * if it is included it must be "T..."
 */
#define RESTRICT_VAR_ARGS(return_type, name, arg_type, ...) \
    template <typename... T>                      \
    typename std::enable_if<_DONT_USE_DIRECTLY::all_same<arg_type, T...>::value, return_type>::type name(__VA_ARGS__)

namespace _DONT_USE_DIRECTLY
{
    template <typename... T>
    struct all_same : std::false_type
    {};

    template <>
    struct all_same<> : std::true_type
    {};

    template <typename T>
    struct all_same<T> : std::true_type
    {};

    template <typename T, typename... Ts>
    struct all_same<T, T, Ts...> : all_same<T, Ts...>
    {};
}  // namespace _DONT_USE_DIRECTLY

