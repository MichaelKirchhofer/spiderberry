/**
 * @file beep.h
 * @brief Provides includes and definitions for raspberry beeper functions
 */
#ifndef _BEEP_H_INCLUDED
#define _BEEP_H_INCLUDED

#include <main.h>

//Namespace declarations
using namespace PiLib;
using namespace std;
using namespace SpiderLib;
using namespace this_thread;
using namespace chrono;

//Function prototypes
void single_beep(int beep_duration);
void sos_beep(int beep_count);
void *walz_beep_thread(void *threadarg);

#endif
