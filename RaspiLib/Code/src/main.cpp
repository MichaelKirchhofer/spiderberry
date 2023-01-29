#include <main.h>

// @brief Main function
// @param int argc, char* argv[] - standard main function parameters
// @return int - status indicator
int main(int argc, char *argv[])
{
  unsigned mVersion, lVersion;
  RaspiLib::LibVersion(mVersion, lVersion);

  cout << WHITE <<"using PiLib Version " << mVersion << "." << lVersion << endl;

  bool isOk = RaspiLib::Startup();
  if (! isOk)
  {
    cerr << "Problems on initializing PiLib - aborting" << endl;
    return -1;
  }
  
  //Init Spider
  cout << "Starting spider runtime !\n";
  Spider mySpider;
  Spider *p_spider = &mySpider;
  ErrorCode err = mySpider.Init();
  //Set spider height
  PiLib::ErrorCode err_raise = mySpider.Raise(-80.);
  
  //Init LED circle
  ws2811_t ledstring = init_led();
  ws2811_t *p_ledstring = &ledstring;
  
  //Go into spider runtime
  //go_walk(mySpider,p_ledstring);
  dance_spider_dance(p_spider,MAX_DANCE_CYCLES);
  
  return 0;
}


