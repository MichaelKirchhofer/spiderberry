#include <Spider.h>
#include <RaspiLib.h>
#include <UltraSound.h>
#include <iostream>
#include <chrono>
#include<thread>

using namespace PiLib;
using namespace std;
using namespace SpiderLib;
using namespace this_thread;
using namespace chrono;


int go_walk(Spider spider);
void lie_down(int sleep_time, Spider spider);
void stand_up(int sleepTime, Spider spider);

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
  
  go_walk(mySpider);

  return 0;
}

//Walk 
int go_walk(Spider spider){

    int init_turn = 0;
    const RaspiPinLabel SpiderTriggerPin = PIN13_GPIO27_PCMD;
    const RaspiPinLabel SpiderEchoPin = PIN15_GPIO22;
    UltraSound usound(SpiderTriggerPin, SpiderEchoPin);
    PiLib::ErrorCode err_raise = spider.Raise(-80.);
    PiLib::ErrorCode err_stop;
    bool err_run;
    int angle = -15;

    while(1){
      //Walk until obstacle is detected
      if(usound.GetDistance() > 40){
        err_run = spider.run(0,-10,1000,0,1);
        init_turn = 1;
      }else if(init_turn){
        //lie_down(1,spider);
        //stand_up(1,spider);
        
        if(angle > 15){
          angle = angle +5;
        }else{
          angle = -15;
        }
        err_run = spider.run(0,-10,1000,angle,1);
        sleep_for(milliseconds(500));
        init_turn = 0;
      }else if(!init_turn && usound.GetDistance() < 40){
        init_turn = 0;
      }
      
    }

}
void lie_down(int sleepTime, Spider spider){

      int step_down = -80;
      while(step_down < 0){
        bool err_raise = spider.Raise(step_down);
        bool err_run = spider.run(0,5,1000,5,1);
        step_down = step_down + 20;
        for(int i = 0; i < 1000000;i++){
        }
      }
      sleep_for(seconds(sleepTime));
}

void stand_up(int sleepTime, Spider spider){

    int step_up = 0;
    while(step_up > -80){
      bool err_raise = spider.Raise(step_up);
        bool err_run = spider.run(0,5,1000,5,1);
        step_up = step_up - 20;
        for(int i = 0; i < 1000000;i++){
        }
      }
      sleep_for(seconds(sleepTime));
}
//Go Right
int go_steps_right(int steps,float angle){
  return 0;
}
//Go Left
int go_steps_left(int steps,float angle){
  return 0;
}
//Go Forward
int go_steps_forward(int steps,float angle){
  return 0;
}
//Go Backward
int go_steps_backward(int steps,float angle){
  return 0;
}
