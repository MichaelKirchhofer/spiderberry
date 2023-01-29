/**
 * @file beep.h
 * @brief Implements raspberry beeper functions
 */
#include <beep.h>

// @brief This function implements the beep sequence for the dancing spider
// using a seperate thread to handle beeping and moving at the same time
// @param void* threadarg - multithreading parameter, thread id and cycle number
// @return none
void *walz_beep_thread(void *threadarg)
{
  struct thread_data_beeper *arg;
  arg = (struct thread_data_beeper*) threadarg;
  
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
