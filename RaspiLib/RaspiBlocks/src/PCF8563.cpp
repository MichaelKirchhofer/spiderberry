#include "PCF8563.h"

#include <RaspiLib.h>
#include <Utils.h>

using namespace PiLib;
using namespace std;

// registers - values are BCD coded
// 0x02 SEC
// 0x03 MIN
// 0x04 HOUR
// 0x05 DAY
// 0x06 WEEKDAY
// 0x07 MONTH
// 0x08 YEAR % 100

PCF8563::PCF8563(int address)
{
  m_address = address;
}

PCF8563::~PCF8563()
{}

ErrorCode PCF8563::SetTime(const TimeInfo &ti)
{
  ByteBuffer bb;

  bb.emplace_back(Utils::toBCD(ti.second % 60));
  bb.emplace_back(Utils::toBCD(ti.minute % 60));
  bb.emplace_back(Utils::toBCD(ti.hour % 24));
  bb.emplace_back(Utils::toBCD(ti.day % 32));
  bb.emplace_back(Utils::toBCD(ti.weekday % 7));
  bb.emplace_back(Utils::toBCD(ti.month % 13));
  bb.emplace_back(Utils::toBCD(ti.year % 100));

  II2c &iface = RaspiLib::GetInstance().GetI2c();

  ErrorCode err = iface.WriteRegister(m_address, SecReg, bb);
  return err;
}

ErrorCode PCF8563::ReadTime(TimeInfo &ti)
{
  II2c &iface = RaspiLib::GetInstance().GetI2c();
  int reg = SecReg;

  ByteBuffer bb;

  // reset timeinfo buffer
  ti.day = ti.hour = ti.minute = ti.month = ti.second = ti.weekday = ti.year = 0;

  ErrorCode err = iface.ReadRegister_RepeatStart(m_address, reg, bb, 7); 
  if (err == E_OK)
  {
    // BCD coded !
    ti.second = Utils::fromBCD(bb[0] & 0x7F);
    ti.minute = Utils::fromBCD(bb[1] & 0x7F);
    ti.hour = Utils::fromBCD(bb[2] & 0x3F);
    ti.day = Utils::fromBCD(bb[3] & 0x3F);
    ti.weekday = Utils::fromBCD(bb[4] & 0x3F);
    ti.month = Utils::fromBCD(bb[5] & 0x1F);
    ti.year = Utils::fromBCD(bb[6]) + 2000;
  }

  return err;
}

static const char *WeekDays[] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};

string PCF8563::ToString(const TimeInfo &ti)
{
  char buf[50];
  sprintf(buf,"%s 20%02d/%02d/%02d %02d:%02d:%02d", WeekDays[ti.weekday % 7], ti.year % 100, ti.month, ti.day, ti.hour, ti.minute, ti.second);

  return buf;
}
