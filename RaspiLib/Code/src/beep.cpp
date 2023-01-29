#include <beep.h>

//Namespace declarations
using namespace PiLib;
using namespace std;
using namespace SpiderLib;
using namespace this_thread;
using namespace chrono;

// @brief This function implements the beep sequence for the dancing spider
// using a seperate thread to handle beeping and moving at the same time
// @param void* threadarg - multithreading parameter, thread id and cycle number
// @return none
void *walz_beep_thread(void *threadarg)
{
  struct thread_data *arg;
  arg = (struct thread_data*) threadarg;
  
  //Calculate the approximate dance duration to adjust beep timings
  int beep_cycles = (arg->beep_cycles * DANCE_CYCLE_TIME_MULTIPLIER)-2;
  
  while(beep_cycles > 0){
    /********************************
    * First tune
    * ******************************/
    single_beep(500);
    sleep_for(milliseconds(250));
    /********************************
    * Second tune
    * ******************************/
    single_beep(150);
    sleep_for(milliseconds(600));
    /********************************
    * Third tune
    * ******************************/
    single_beep(100);
    sleep_for(milliseconds(600));
    beep_cycles--;
  }
  pthread_exit(NULL);
  
}

// @brief This function initializes the LED on the back of the spider
// @param none
// @return none
ws2811_t init_led(){

  cout << CYAN <<"Init LED circle!\n";
  
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

// @brief This function implements the lying down feature the spiderbot
// @param bool 
// @return none
void single_beep(int beep_duration)
{
  IPin *m_buzzer_pin = nullptr;
  IPin &buzzer_pin = RaspiLib::GetInstance().GetPin(PIN11_GPIO17);
  m_buzzer_pin = &buzzer_pin;

  buzzer_pin.SetPinMode(FSEL_OUTP);

  m_buzzer_pin->WriteBool(true);
  sleep_for(milliseconds(beep_duration));
  m_buzzer_pin->WriteBool(false);
  
}

// @brief This function implements the lying down feature the spiderbot
// @param int beep_count - number of sos signal cycles
// @return none
void sos_beep(int beep_count){
  
  cout << YELLOW << "Started SOS beeping!\n";
  
  for(int i = 0; i< beep_count;i++){
    /********************************
    * Short beep section
    * ******************************/
    // first beep
    single_beep(100);
    sleep_for(milliseconds(100));
    // second beep
    single_beep(100);
    sleep_for(milliseconds(100));
    // third beep
    single_beep(100);
    sleep_for(milliseconds(500));
    /********************************
    * Long beep section
    * ******************************/
    // first beep
    single_beep(300);
    sleep_for(milliseconds(300));
    // second beep
    single_beep(300);
    sleep_for(milliseconds(300));
    // third beep
    single_beep(300);
    sleep_for(milliseconds(500));
    /********************************
    * Short beep section
    * ******************************/
    // first beep
    single_beep(100);
    sleep_for(milliseconds(100));
    // second beep
    single_beep(100);
    sleep_for(milliseconds(100));
    // third beep
    single_beep(100);
    /********************************
    * Wait after Signal
    * ******************************/
    sleep_for(milliseconds(3000));

  }
}
