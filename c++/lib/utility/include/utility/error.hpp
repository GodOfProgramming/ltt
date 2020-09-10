#pragma once
#include <any>
#include <optional>
#include <ostream>
#include <iostream>

namespace utility
{
  template <typename E>
  class Error
  {
   public:
    Error() = default;

    Error(E t)
    {
      mValue.emplace(t);
    }

    static auto none() -> Error
    {
      return Error();
    }

    auto operator*() -> E
    {
      return *mValue;
    }

    auto exists() -> bool
    {
      return mValue.has_value();
    }

   private:
    std::optional<E> mValue;
  };

  template <typename E>
  auto operator<<(std::ostream& stream, Error<E>& err) -> std::ostream&
  {
    if (err.exists()) {
      return stream << *err;
    } else {
      return stream << "none";
    }
  }
}  // namespace utility