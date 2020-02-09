#pragma once
#include <regex>

namespace utility
{
  inline std::vector<std::string> StringSplit(const std::string str, const std::regex delim);

  inline std::vector<std::string> StringSplit(const std::string str, const std::string regex);

  inline std::vector<std::string> StringSplit(const std::string str, const std::regex delim)
  {
    std::sregex_token_iterator start(str.begin(), str.end(), delim, -1);
    std::sregex_token_iterator end;
    return std::move(std::vector<std::string>(start, end));
  }

  inline std::vector<std::string> StringSplit(const std::string str, const std::string regex)
  {
    std::regex delim(regex);
    return StringSplit(str, delim);
  }
}  // namespace utility

