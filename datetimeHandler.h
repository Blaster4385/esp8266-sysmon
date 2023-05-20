#ifndef __DATETIMEHANDLER_H__
#define __DATETIMEHANDLER_H__

#include<Arduino.h>

extern uint8_t localTemp;
extern uint8_t localHum;
extern uint8_t hour;
extern uint8_t minute;
extern uint8_t time_tab[6];
extern void setTime(String time);

#endif // __DATETIMEHANDLER_H__
