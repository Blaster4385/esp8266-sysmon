#include "datetimeHandler.h"

uint8_t localTemp = 255;
uint8_t localHum = 255;
uint8_t hour = 0;
uint8_t minute = 0;
uint8_t time_tab[6] = {'-', '-', ':', '-', '-', 0};

void setTime(String time)
{
  if(time.length() == 8)
  {
    hour = time.substring(0,2).toInt();
    time.substring(0,5).getBytes(time_tab, 5+1);
    minute = time.substring(3,5).toInt();
  }
}


