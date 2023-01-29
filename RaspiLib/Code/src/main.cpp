#include <Spider.h>
#include <RaspiLib.h>
#include <UltraSound.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <cstdlib>
#include <pthread.h>
#include <string>


/* LED lib */
#include "ws2811.h" 
#include "adcSample.h"

//Cout defines
#define BLACK   "\033[0m"
#define WHITE   "\033[1;37m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"


//User defines
#define MAX_TURN_ANGLE 20
#define TURN_STEP 5
#define DANCE_STEP 2
#define MAX_DANCE_CYCLES 5
#define DANCE_CYCLE_TIME_MULTIPLIER 3
#define SPIDER_SPEED 500


struct thread_data{
  int thread_id;
  int beep_cycles;
};

//Namespace declarations
using namespace PiLib;
using namespace std;
using namespace SpiderLib;
using namespace this_thread;
using namespace chrono;

//Function prototypes
ws2811_t init_led();
int  go_walk(Spider spider,ws2811_t *ledstring);
void lie_down(int sleep_time, Spider *spider);
void stand_up(int sleepTime, Spider *spider);
void single_beep(int beep_duration);
void sos_beep(int beep_count);
void spider_wave(Spider *spider);
void dance_spider_dance(Spider *spider,int dance_cycles);
void *walz_beep_thread(void *threadarg);


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


// @brief This function implements the basic walking and evasion procedures
// @param Spider spider - The spider object
// @return int - status indicator
int go_walk(Spider *spider, ws2811_t *ledstring){

    int init_turn = 0;
    const RaspiPinLabel SpiderTriggerPin = PIN13_GPIO27_PCMD;
    const RaspiPinLabel SpiderEchoPin = PIN15_GPIO22;
    UltraSound usound(SpiderTriggerPin, SpiderEchoPin);
    PiLib::ErrorCode err_stop;
    bool err_run;
    int angle = - MAX_TURN_ANGLE;

    int turn_tries = 0;

    while(1){
      //Walk until obstacle is detected
      while(usound.GetDistance() > 50){
        
        err_run = spider->run(0,-25,SPIDER_SPEED,0,1);
        init_turn = 1;
        
        for (int i = 0; i < ledstring->channel[0].count; i++)
        {
          ledstring->channel[0].leds[i] = 0xFF0000;
        }
              ws2811_render(ledstring);
      
        //ws2811_fini(ledstring);
      }
      
      cout << "Obstacle detected ! Execute evasion protocol\n";
      //Obstacle detected
      while(init_turn){
          
          //Beep once to notify about found obstacle
          single_beep(500);
          
          //Init danger led ring
          init_led();
        
          //Try differrent angles
          if(angle < MAX_TURN_ANGLE){
            angle = angle + TURN_STEP;
            turn_tries ++;
          }else{
            turn_tries ++;
            angle = -MAX_TURN_ANGLE;
          }

          //After number of tries, go to floor level and beep in an SOS pattern
          if(turn_tries > 5){
            lie_down(1,spider);
            turn_tries = 0;
            cout << "Obstacle could't be avoided, SOS!\n";
            sos_beep(2);
          }
          //Try evading the obstacle
          err_run = spider->run(0,0,SPIDER_SPEED,angle,1);
          sleep_for(milliseconds(1000));
         
        //Break out of evasion pattern  
        if(usound.GetDistance() > 60){
          cout << "Obstacle avoided, continuing to run!\n";
          init_turn = 0;
          angle = -15;
          turn_tries = 0;
        }
     }
  }
  ws2811_fini(ledstring);
  return 0;

}

// @brief This function implements the lying down feature the spiderbot
// @param none
// @return none
void lie_down(int sleepTime, Spider *spider){

      int step_down = -80;
      while(step_down < 0){
        bool err_raise = spider->Raise(step_down);
        bool err_run = spider->run(0,0,1000,0,1);
        step_down = step_down + 20;
        for(int i = 0; i < 1000000;i++){
        }
      }
    sleep_for(seconds(sleepTime));
}



// @brief This function implements the standing up feature the spiderbot
// @param none
// @return none
void stand_up(int sleepTime, Spider *spider){

    int step_up = 0;
    while(step_up > -80){
      bool err_raise = spider->Raise(step_up);
        bool err_run = spider->run(0,0,1000,0,1);
        step_up = step_up - 20;
        for(int i = 0; i < 1000000;i++){
        }
      }
      sleep_for(seconds(sleepTime));
}

// @brief This function handles the waving of the spider frontleg
// @param Spider spider - spider class object
// @return none
void spider_wave(Spider *spider){
  
     

}
// @brief This function implements a walz dance routine using the run
// function and the beeper as a companion melody
// @param Spider spider - pointer to spider object, 
// int dance_cycles - number of dance cycles  
// @return none
void dance_spider_dance(Spider *spider,int dance_cycles){
  
  int cycles = 0;
  pthread_t walz_thread;
  struct thread_data data;
  data.thread_id = 1;
  data.beep_cycles = dance_cycles;
  // Set dance cycle according to user input or set max if user input
  cycles = dance_cycles;
  cout << WHITE <<"Spider is going to be dancing for: "<< cycles <<" dance cycles!\n";
  
  bool err_run = false;
  int steps = DANCE_STEP;
  
  int thread_create = pthread_create(&walz_thread,NULL,walz_beep_thread,(void *)&data);
  
  if(thread_create){
    cerr << "Error while creating walz beeper thread!\n";
    cerr << "Dancing without beep should be possible!\n";
  }else{
    cout << GREEN << "Detached beep thread from main runtime!\n";
  }
  
  while(cycles > 0){
    cout << WHITE <<"Legs foward, Cycles remaining: "<< cycles << " \n";
    while(1){
      err_run = spider->run(0,-25,SPIDER_SPEED,0,1);
      if(steps > 0){
        steps--;
      }else{
        break;
      }
    }
    steps = DANCE_STEP;
    cout << "Go backwards \n";
    while(1){
      err_run = spider->run(0,20,SPIDER_SPEED,5,1);
      if(steps > 0){
        steps--;
      }else{
        break;
      }
    }
    cycles--;
    steps = DANCE_STEP ;
  }
  
  void *status;
  int thread_join = pthread_join(walz_thread, &status);
  
  if(thread_join != 0){
    cerr <<  "Error in joining beep thread back into main runtime!\n";
  }else{
    cout << GREEN << "Successfully joined main and secondary threads!\n";
  }
  
  pthread_exit(NULL);
  lie_down(10,spider);
  
}

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
