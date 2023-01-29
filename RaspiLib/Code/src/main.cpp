/**
 * @file main.c
 * @brief Provides program entry point and user interface
 */
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
    
  
  return is_running;
}

// @brief User input handler function
// @param none
// @return int state - exit condition
int handle_user_io(){

  //Init Spider
  Spider mySpider;
  Spider *p_spider = &mySpider;
  ErrorCode err = mySpider.Init();
  //Set spider height
  PiLib::ErrorCode err_raise = mySpider.Raise(-80.);
  stand_up(1,p_spider);
  
  //Init LED circle
  ws2811_t ledstring = init_led();
  ws2811_t *p_ledstring = &ledstring;
  cout << GREEN << "Spider init complete, start user IO \n";   
  
  cout << CYAN << "----------------------------------------\n";
  cout << "Welcome to the SPIDERBERRY program!\n";
  err = mySpider.waveLeg(5,20,80,3);
  sleep_for(milliseconds(500));
  
  //Operation parameters:
  int is_running = 1;
  int user_input;
  //Wave 
  int wave_counter,wave_start_pos,wave_length,
  wave_offset,wave_stop_pos,leg_index;
  //Dance
  int dance_cycles;
  //Beep
  int number_of_beeps;
  //Walk
  int obstacles_until_quit;
  
  while(is_running){
    
    print_user_menu();
    cin >> user_input;
    
    switch (user_input){
      /********************************
      * Wave
      * ******************************/
      case 1:
        // Wave counter
        cout << CYAN << "\n---------- WAVE MODE ----------\n";
        cout <<"\nHow many waves do you want to see? (Nr. 1-99)\n";
        cout << "Wave counter: ";
        cin >> wave_counter;
        if(wave_counter < 0 || wave_counter > 99){
          cout << RED <<"Wave counter out of bounds, using 5 instead";
          wave_counter = 5;
        }
        // Wave positional data
        cout << CYAN <<"Wave start position (0 - 170)?\nNumber: ";
        cin >> wave_start_pos;
        if(wave_start_pos < 0 || wave_start_pos > 170){
          cout << RED << "Wave start position out of bounds, using 20 instead!\n";
          wave_start_pos = 20;
        }
        wave_offset = 170 - wave_start_pos;
        //How far should be waved
        cout << CYAN << "How far should i wave ? (10 - "<< wave_offset << ") \nNumber:";
        cin >> wave_length;
        wave_stop_pos = wave_length + wave_start_pos;
        
        if(wave_stop_pos < 0 || wave_stop_pos > 180){
          cout << RED <<  "Wave length out of bounds, using 80 instead!\n";
          wave_stop_pos = 80;
        }
        //Which leg should wave
        cout << CYAN <<"Which leg do you want let wave?\n 0 - Front Left |";
        cout << "1 - Middle Left | 2 - Back Left\n";
        cout << "3 - Front Right | 4 - Middle Right | 5 - Back Right\n";
        cout << "Enter leg number: ";
        cin >> leg_index;
        
        if(leg_index < 0  || leg_index > 5){
          cout << RED << "Wrong leg index, using 3 instead !\n";
        }
        mySpider.waveLeg(wave_counter,wave_start_pos,wave_stop_pos,leg_index);
        break;
        
      /********************************
      * Dance Spider Dance !!
      ********************************/ 
      case 2:
        cout << CYAN << "\n---------- DANCE SPIDER DANCE----------\n";
        cout <<  "\nHow many dance cylces should I do ? (2 - 99)\n";
        cout << "Number of cycles: ";
        cin >> dance_cycles;
        if(dance_cycles < 2 || dance_cycles > 99){
          cout << RED << "Dance cycles out of bounds, using 5 instead!\n";
        }
        dance_spider_dance(p_spider,dance_cycles);
        stand_up(1,p_spider);
        break;
        
      /********************************
      * Walk and Evade
      ********************************/  
      case 3:
        cout << CYAN << "\n---------- WALK AND EVADE MODE ----------\n";
        cout << "Spider will shutdown after set number of unavigable object have been encountered!\n";
        cout << "Please enter shutdown threshold number: ";
        cin >> obstacles_until_quit;
        cout <<"\nStarting walking and obstacle evasion procedure until "<<
        obstacles_until_quit<< " number of objects encountered!\n";
        go_walk(p_spider,p_ledstring,obstacles_until_quit);
        break;
      /********************************
      * SOS Beep
      ********************************/ 
      case 4:
        cout << CYAN << "\n---------- SOS BEEP MODE ----------\n";
        cout << "\nHow many beeps do you want ?(1 - 99)\n";
        cout << "Number of Beeps: ";
        cin >>  number_of_beeps;
        if(number_of_beeps < 0 || number_of_beeps > 99){
          cout << RED << "Number of beeps out of bounds, using 5 instead!\n";
          number_of_beeps = 5;
        }
        sos_beep(number_of_beeps);
        break;
        
      /********************************
      * Exit
      ********************************/   
      case 99:
        cout << "Thank you for using SPIDERBERRY!\n";
        err = mySpider.waveLeg(5,20,80,3);
        lie_down(0,p_spider);
        is_running = false;
        break;
        
      /********************************
      * Default (Display Menu)
      ********************************/    
      default:
        print_user_menu();
        cin >> user_input;
        break;
    }
    
  }

  //dance_spider_dance(p_spider,4);
  //go_walk(p_spider,p_ledstring);
  
  //Go into spider runtime
  
  return 0;
}

// @brief This function prints out the top level user menu
// @param none
// @return none
void print_user_menu(){
   cout << CYAN << "----------------------------------------\n";
   cout << "Please choose a function out of the list !\n";
   cout << "Function List:\n1 - Wave \n2 - Dance\n";
   cout << "3 - Walk and detect obstacles\n";
   cout << "4 - Set off SOS beep pattern\n";
   cout << "99 - Exit program and shutdown spiderbot!\n";
   cout << "----------------------------------------\n";
   cout << "Enter function code: ";
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
