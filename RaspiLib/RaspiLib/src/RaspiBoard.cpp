#include "RaspiBoard.h"
#include "Pin.h"

#include <stdio.h>
#include <cstring>

#include "Utils.h"

// we use the BCM2835 lib here
#include "bcm2835.h"

using namespace PiLib;
using namespace std;

std::string BoardInfo::ToString() const
{
  string tmp = model;
  
  char hexstring[30];
  sprintf(hexstring, "0x%08X", revision);
  tmp += " Revision " + string(hexstring);

  tmp += " Processor ";
  tmp += (processor == PI_PROCESSOR_BCM2835) ? "BCM2835" : "unknown";

  return tmp;
}

// RaspiBoard functions

RaspiBoard::RaspiBoard()
{
  bcm2835_init();
}

RaspiBoard::~RaspiBoard()
{
  bcm2835_close();
}

ErrorCode RaspiBoard::Init()
{
  ErrorCode res = ReadBoardInfo();
  return res;
}

ErrorCode RaspiBoard::ReadBoardInfo()
{
  FILE *fp = nullptr;
  char line[120];
  string revision;
  string hardware;
  string model;

  // extract info from /proc/cpuinfo
  fp = fopen("/proc/cpuinfo","r");
  if (! fp)
  {
    return E_NOACCESS_CPUINFO;
  }

  while (fgets(line, 120, fp))
  {
    if (strncmp(line, "Hardware", 8) == 0)
      hardware = line;
    if (strncmp(line, "Revision", 8) == 0)
      revision = line;
    if (strncmp(line, "Model", 5) == 0)
      model = line;
  }

  fclose(fp);

  // read Hardware
  if (hardware.length() < 10)
    return E_MISSING_CPUINFO;

  if (hardware.find("BCM2835") != string::npos) m_board.processor = PI_PROCESSOR_BCM2835;
  else m_board.processor = PI_PROCESSOR_UNKNOWN;

  vector<string> items = Utils::split(model, ':');
  if (items.size() > 1)
    m_board.model = Utils::trim(items[1]);

  // read Revision
  // isolate revision number

  // skip to ':'
  items = Utils::split(revision, ':');
  // take second item
  if (items.size() > 1)
  {
    string number = Utils::trim(items[1]);
    
    char *pend = nullptr;
    m_board.revision = strtol(number.c_str(), &pend, 16);
  }

  return E_OK;
}
 
