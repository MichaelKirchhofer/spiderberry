#include "Utils.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

#include <thread>

using namespace std;

#ifdef _WIN32
// for exact time
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif

namespace Utils
{

  string ltrim(string s)
  {
    s.erase(s.begin(), find_if(s.begin(), s.end(), [](int x){return isspace(x);}));
    return s;
  }

  string rtrim(string s)
  {
    s.erase(find_if(s.rbegin(), s.rend(), [](int x){return isspace(x);}).base(), s.end());
    return s;
  }

  vector<string> split(string s, char delimiter)
  {
    stringstream ss(s);
    string item;
    vector<string> result;

    while (getline(ss, item, delimiter))
      result.push_back(item);

    return result;
  }

  void delay_ms(unsigned milli)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(milli));
    // alternative: bcm2835_delay(unsigned milli);
  }

  void delay_us(unsigned micro)
  {
    std::this_thread::sleep_for(std::chrono::microseconds(micro));
    // alternative: bcm2835_delayMicroseconds(uint64_t micros);
  }

  int fromBCD(int bcdCode)
  {
    bcdCode = bcdCode % 256;

    int result = (bcdCode & 0x0F) + (bcdCode & 0xF0) / 16 * 10;
    return result;
  }

  int toBCD(int value)
  {
    if (value < 0) return 0;

    value = value % 100;
    int result = value % 10 + (value / 10) % 10 * 16;

    return result;
  }

  // secs.usecs
  uint64_t getExactTime_us()
  {
    uint64_t timeval = 0.0;

#ifdef _WIN32

    // this is for exact time in ms
    struct _timeb ttime;
    _ftime(&ttime);
    time_t now = ttime.time;

    timeval = now * 1000000 + ttime.millitm * 1000;

#else

  struct timeval ctime;
  struct timezone tzone;

  gettimeofday(&ctime, &tzone);

  time_t now = ctime.tv_sec;

  timeval = now * 1000000 + ctime.tv_usec;

#endif

    return timeval;
  }

};