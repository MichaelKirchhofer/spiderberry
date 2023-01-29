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
  
  int is_running = handle_user_io();
  
  return 0;
}

int handle_user_io(){

  //Init Spider
  Spider mySpider;
  Spider *p_spider = &mySpider;
  ErrorCode err = mySpider.Init();
  //Set spider height
  PiLib::ErrorCode err_raise = mySpider.Raise(-80.);
  
  //Init LED circle
  ws2811_t ledstring = init_led();
  ws2811_t *p_ledstring = &ledstring;
  cout << GREEN << "Spider init complete, start user IO \n";   
 
  //err = mySpider.waveLeg(5,20,80,3);
  //dance_spider_dance(p_spider,4);
  //go_walk(p_spider,p_ledstring);
  
  //Go into spider runtime
  
  return 0;
}


// @brief This function initializes the LED on the back of the spider
// @param none
// @return none
ws2811_t init_led(){

  cout <<"Init LED circle!\n";
  
  ws2811_return_t ret;
  //ws2811_t ledstring = {0,0,(const rpi_hw_t*)WS2811_TARGET_FREQ,10,18,0,7,125,WS2811_STRIP_RGB,0};
  ws2811_t ledstring = {};
  ledstring.freq = WS2811_TARGET_FREQ; // 800000 Hz
  ledstring.dmanum = 10; // DMA
  ws2811_channel_t channel = {};
  channel.gpionum = 18; // Pin 12 (GPIO 18)
  channel.count = 7; // number of LEDs
  channel.invert = 0;
  channel.brightness = 255;
  
  ledstring.channel[0] = channel;
  
  if((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS)
  {
    cerr << "ws2811_init failed:  " << ws2811_get_return_t_str(ret) << " \n";
  }
  
  return ledstring;

}
