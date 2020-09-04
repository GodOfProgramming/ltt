#pragma once

namespace util
{
  using EmptyResult = std::tuple<>;
  template <typename OkType, typename ErrType>
  class Result
  {
   public:
    static auto Ok(OkType ok) -> Result
    {
      Result res;
      res.ok.emplace(ok);
      return res;
    }

    static auto Err(ErrType err) -> Result
    {
      Result res;
      res.err.emplace(err);
      return res;
    }

    auto is_ok() -> bool
    {
      return this->ok.has_value();
    }

    auto is_err() -> bool
    {
      return this->err.has_value();
    }

    auto unwrap_ok() -> OkType
    {
      return *this->ok;
    }

    auto unwrap_err() -> ErrType
    {
      return *this->err;
    }

   private:
    std::optional<OkType> ok;
    std::optional<ErrType> err;
  };
}  // namespace util