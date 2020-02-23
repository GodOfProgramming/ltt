#pragma once
#include <regex>

#include <cinttypes>
#include <climits>

namespace utility
{
  bool IsPosNumber(const std::string& str);

  bool IsNumeric(const std::string& str);

  template <typename T>
  bool StringParse(const std::string& str, T& output);

  std::vector<std::string> StringSplit(const std::string& str, const std::regex delim);

  std::vector<std::string> StringSplit(const std::string& str, const std::string regex);

  inline bool IsPosNumber(const std::string& str)
  {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
  }

  inline bool IsNumeric(const std::string& str)
  {
    char* p;
    ::strtol(str.c_str(), &p, 10);
    return !str.empty() && *p == '\0';
  }

  template <>
  inline bool StringParse(const std::string& str, uint8_t& output)
  {
    unsigned long long out;

    if (!IsPosNumber(str)) {
      return false;
    }

    out = std::stoull(str);

    if (out > UCHAR_MAX) {
      return false;
    }

    output = out;

    return true;
  }

  template <>
  inline bool StringParse(const std::string& str, uint16_t& output)
  {
    unsigned long long out;

    if (!IsPosNumber(str)) {
      return false;
    }

    out = std::stoull(str);

    if (out > USHRT_MAX) {
      return false;
    }

    output = out;

    return true;
  }

  inline std::vector<std::string> StringSplit(const std::string& str, const std::regex delim)
  {
    std::sregex_token_iterator start(str.begin(), str.end(), delim, -1);
    std::sregex_token_iterator end;
    return std::vector<std::string>(start, end);
  }

  inline std::vector<std::string> StringSplit(const std::string& str, const std::string regex)
  {
    std::regex delim(regex);
    return StringSplit(str, delim);
  }
}  // namespace utility

