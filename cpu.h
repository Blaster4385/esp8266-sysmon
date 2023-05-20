#ifndef __CPU_H__
#define __CPU_H__

#include<Arduino.h>

#define defaultValue 0

extern void initCpuParameters();
extern int8_t setCpuTemp(String command);
extern int8_t setCpuUsage(String command);
extern int8_t setRamMax(String command);
extern int8_t setRamFree(String command);

extern int16_t cpuUsageGlobal;
extern int16_t cpuUsage[8];
extern int16_t cpuTempGlobal;
extern int16_t cpuTemp[8];
extern uint32_t ramMax;
extern uint32_t ramFree;

#endif // __CPU_H__
