#pragma once
#include <regex>

namespace utility
{
  inline std::vector<std::string> StringSplit(const std::string str, const std::string regex)
  {
    std::regex delim(regex);
    std::sregex_token_iterator start(str.begin(), str.end(), delim, -1);
    std::sregex_token_iterator end;
    return std::move(std::vector<std::string>(start, end));
  }
}  // namespace utility

