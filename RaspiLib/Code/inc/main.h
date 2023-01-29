#ifndef _MAIN_H_INCLUDED
#define _MAIN_H_INCLUDED

//Spider includes
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
#include <move.h>
#include <beep.h>

// LED lib
#include "ws2811.h" 
#include "adcSample.h"

//User defines
#define MAX_TURN_ANGLE 20
#define TURN_STEP 5
#define DANCE_STEP 2
#define MAX_DANCE_CYCLES 5
#define DANCE_CYCLE_TIME_MULTIPLIER 3
#define SPIDER_SPEED 500

//Cout defines
#define BLACK   "\033[0m"
#define WHITE   "\033[1;37m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[1;36m"


//Namespace declarations
using namespace PiLib;
using namespace std;
using namespace SpiderLib;
using namespace this_thread;
using namespace chrono;

//Multithreading parameter struct
struct thread_data{
  int thread_id;
  int beep_cycles;
};

//Function prototypes
ws2811_t init_led();
int handle_user_io();
void print_user_menu();

#endif
