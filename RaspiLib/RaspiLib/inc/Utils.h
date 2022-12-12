#pragma once

#include <string>
#include <vector>

namespace Utils
{
  std::string ltrim(std::string s);
  std::string rtrim(std::string s);

  inline std::string trim(std::string s) { return ltrim(rtrim(s)); }

  std::vector<std::string> split(std::string s, char delimiter);

  void delay_ms(unsigned milli);
  void delay_us(unsigned micro);

  int fromBCD(int bcdCode);
  int toBCD(int value);

  // secs.usecs
  uint64_t getExactTime_us();
}