#pragma once
#include <optional>

namespace utility
{
  template <typename E>
  class Error
  {
   public:
    Error() = default;

    Error(E t)
    {
      this->value.emplace(t);
    }

    static auto none() -> Error<{}>
    {
      return Error<{}>()
    }

    auto operator*() -> E
    {
      return *this->value;
    }

    auto is_err() -> bool
    {
      return this->value.has_value;
    }

   private:
    std::optional<E> value;
  };
}  // namespace utility