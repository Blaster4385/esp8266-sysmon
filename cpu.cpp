#include "cpu.h"

int16_t cpuUsageGlobal = defaultValue;
int16_t cpuUsage[8] = {defaultValue, defaultValue, defaultValue,defaultValue, defaultValue, defaultValue, defaultValue, defaultValue};
int16_t cpuTempGlobal = defaultValue;
int16_t cpuTemp[8] = {defaultValue, defaultValue, defaultValue,defaultValue, defaultValue, defaultValue, defaultValue, defaultValue};
uint32_t ramFree = defaultValue;
uint32_t ramMax = defaultValue;

static int8_t extractArray(String &command, int16_t *array)
{
    int8_t startIndex = 1, lastIndex;
    int8_t index = 0;
    bool needStop = false;
    while(index < 8)
    {
      lastIndex = command.indexOf(",", startIndex);
      if(lastIndex == -1)
      {
        lastIndex = command.length() - 1;
        needStop = true;
      }

      array[index++] = command.substring(startIndex, lastIndex).toInt();
      if(needStop)
        break;
      startIndex = lastIndex +1;
    }
    return index;
}

int8_t setCpuTemp(String command)
{
  if(command.substring(0, 1) == "[")
  {
    extractArray(command, cpuTemp);
  }
  else
  {
    cpuTempGlobal = command.toInt();
  }
  return 0;
}

int8_t setCpuUsage(String command)
{
  if(command.substring(0, 1) == "[")
  {
    extractArray(command, cpuUsage);
  }
  else
  {
    cpuUsageGlobal = command.toInt();
  }
  return 0;
}

int8_t setRamMax(String command)
{
  ramMax = command.toInt();
  return 0;
}

int8_t setRamFree(String command)
{
  ramFree = command.toInt();
  return 0;
}

