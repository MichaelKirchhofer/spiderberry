/**
 * @file main.cpp
 * @brief Implements move functions for the spiderbot
 */
#include <move.h>

// @brief This function implements the basic walking and evasion procedures
// @param Spider spider - The spider object
// @return int - status indicator
int go_walk(Spider *spider, ws2811_t *ledstring, int obstacles_until_quit){

	//Preparations for run / init run variables
    int init_turn = 0;
    int angle = - MAX_TURN_ANGLE;
    int turn_tries = 0;
    int obstacles;
    PiLib::ErrorCode err_stop;
    bool err_run;
    //Set spider height
	PiLib::ErrorCode err_raise = spider->Raise(-80.);
    
    //Init usound distance sensor
    const RaspiPinLabel SpiderTriggerPin = PIN13_GPIO27_PCMD;
    const RaspiPinLabel SpiderEchoPin = PIN15_GPIO22;
    UltraSound usound(SpiderTriggerPin, SpiderEchoPin);
    
	while (obstacles < obstacles_until_quit){
      //Walk until obstacle is detected
      while(usound.GetDistance() > 50){
        
        err_run = spider->run(0,-25,SPIDER_SPEED,0,1);
        init_turn = 1;
        
        for (int i = 0; i < ledstring->channel[0].count; i++)
        {
          ledstring->channel[0].leds[i] = 0x00FF00; //GREEN 
        }
              ws2811_render(ledstring);
      
        //ws2811_fini(ledstring);
      }
      
      cout << RED << "Obstacle detected ! Execute evasion protocol\n";
      
      //Obstacle detected
      while(init_turn){
          
          //Beep once to notify about found obstacle
          single_beep(500);
      
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
			
			obstacles++;
            if(obstacles == obstacles_until_quit){
				break;
			}
			
            lie_down(1,spider);
            turn_tries = 0;
            cout << RED << "Obstacle could't be avoided, SOS!\n";
            cout << "Obstacles remaining until shutdown to menu: "<<
            obstacles_until_quit-obstacles<< " !\n";
            sos_beep(2);
            
          }
          //Try evading the obstacle
          err_run = spider->run(0,0,SPIDER_SPEED,angle,1);
          sleep_for(milliseconds(1000));
         
        //Break out of evasion pattern  
        if(usound.GetDistance() > 60){
          cout << GREEN <<"Obstacle avoided, continuing to run!\n";
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
	
	  cout << WHITE << "Spider lying down and sleeping for "<< sleepTime <<" seconds\n";
      int step_down = -80;
      while(step_down < 0){
        bool err_raise = spider->Raise(step_down);
        bool err_run = spider->run(0,0,1000,0,1);
        step_down = step_down + 20;
        sleep_for(milliseconds(20));
      }
    sleep_for(seconds(sleepTime));
}

// @brief This function implements the standing up feature the spiderbot
// @param none
// @return none
void stand_up(int sleepTime, Spider *spider){
	
	 cout << WHITE << "Spider standing up and sleeping for "<< sleepTime <<" seconds\n";
	
    int step_up = 0;
    while(step_up > -80){
      bool err_raise = spider->Raise(step_up);
        bool err_run = spider->run(0,0,1000,0,1);
        step_up = step_up - 20;
        sleep_for(milliseconds(20));
      }
      sleep_for(seconds(sleepTime));
}

// @brief This function implements a walz dance routine using the run
// function and the beeper as a companion melody
// @param Spider spider - pointer to spider object, 
// int dance_cycles - number of dance cycles  
// @return none
void dance_spider_dance(Spider *spider,int dance_cycles){
  
  int cycles = 0;
  pthread_t walz_thread;
  struct thread_data_beeper data;
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
  
  lie_down(10,spider);
  
  void *status;
  int thread_join = pthread_join(walz_thread, &status);
  
  if(thread_join != 0){
    cerr <<  "Error in joining beep thread back into main runtime!\n";
  }else{
    cout << GREEN << "Successfully joined main and secondary threads!\n";
  }
  
}
