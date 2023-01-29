#ifndef _MOVE_H_INCLUDED
#define _MOVE_H_INCLUDED

#include <main.h>
#include "ws2811.h" 

//Namespace declarations
using namespace PiLib;
using namespace std;
using namespace SpiderLib;
using namespace this_thread;
using namespace chrono;

int  go_walk(Spider *spider,ws2811_t *ledstring);
void lie_down(int sleep_time, Spider *spider);
void stand_up(int sleepTime, Spider *spider);
void spider_wave(Spider *spider);
void dance_spider_dance(Spider *spider,int dance_cycles);
#endif
