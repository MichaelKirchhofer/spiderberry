#include <Spider.h>
#include <RaspiLib.h>

#include <iostream>

using namespace PiLib;
using namespace std;
using namespace SpiderLib;

int main(int argc, char *argv[])
{
  unsigned mVersion, lVersion;
  RaspiLib::LibVersion(mVersion, lVersion);

  cout << "using PiLib Version " << mVersion << "." << lVersion << endl;

  bool isOk = RaspiLib::Startup();
  if (! isOk)
  {
    cerr << "Problems on initializing PiLib - aborting" << endl;
    return -1;
  }

  Spider mySpider;
  ErrorCode err = mySpider.Init();

  return 0;
}
