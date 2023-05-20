#include "gfx_1.h"
#include "datetimeHandler.h"
#include "cpu.h"
#include "Adafruit_GFX.h"
#include <Adafruit_ILI9341.h>

static uint8_t s_time_tab[6] = { '0', '0', ':', '0', '0', 0 };
static uint8_t s_localHum = 0, s_localTemp = 0, s_cpuTemp = 255;
static int16_t s_cpu_usage = 255;
static uint32_t s_ram_free = 255, s_ram_max = 255;
static String s_os = "Unknown", s_kernel = "Unknown", s_uptime = "Unknown";


// 5px size
static void gfx_update_toolbar();
static void gfx_update_cpu();
static void gfx_update_mem();
static void gfx_update_sys_info();

static void writeArray(const char *buf) {
  uint8_t c = 0;
  while (buf[c] != 0)
    tft.write(buf[c++]);
}


void gfx1_udpate() {
  gfx_update_toolbar();
  gfx_update_cpu();
  gfx_update_mem();
  gfx_update_sys_info();
}

#define LINE_Y_REL 29
#define TEXT_Y_REL 9
#define FIRST_TEXT_Y_REL 40
#define TEXT_ESPACE_Y 35
#define TEXT_ESPACE_SHORT_Y 25

char buf[20];

void gfx1_layout() {
  uint8_t c = 0;

  tft.fillScreen(ILI9341_BLACK);
  tft.drawLine(10, 18, 310, 18, ILI9341_YELLOW);
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);

  // Temp
  tft.setCursor(35, 0);
  tft.setTextSize(1);
  tft.write('o');
  tft.setTextSize(2);
  tft.setCursor(35 + 8, 2);
  tft.write('C');

  // Humidity
  tft.setCursor(300, 2);
  tft.write('%');

  // CPU
  tft.setTextSize(2);
  tft.drawRoundRect(5, 25, 150, 90, 10, ILI9341_RED);
  tft.drawLine(5, 25 + LINE_Y_REL, 154, 25 + LINE_Y_REL, ILI9341_RED);
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  tft.setCursor(60, 25 + TEXT_Y_REL);
  sprintf(buf, "CPU");
  writeArray(buf);
  tft.setCursor(15, 25 + FIRST_TEXT_Y_REL);
  sprintf(buf, "Usage:");
  writeArray(buf);
  tft.setCursor(15, 25 + FIRST_TEXT_Y_REL + TEXT_ESPACE_SHORT_Y);
  sprintf(buf, "Temp:");
  writeArray(buf);
  tft.setCursor(15, 25 + FIRST_TEXT_Y_REL + TEXT_ESPACE_SHORT_Y * 2);

  // memory
  tft.drawRoundRect(165, 25, 150, 90, 10, ILI9341_GREEN);
  tft.drawLine(165, 25 + LINE_Y_REL, 314, 25 + LINE_Y_REL, ILI9341_GREEN);
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.setCursor(205, 25 + TEXT_Y_REL);
  sprintf(buf, "MEMORY");
  writeArray(buf);
  tft.setCursor(175, 25 + FIRST_TEXT_Y_REL);
  sprintf(buf, "Max:");
  writeArray(buf);
  tft.setCursor(175, 25 + FIRST_TEXT_Y_REL + TEXT_ESPACE_SHORT_Y);
  sprintf(buf, "Free:");
  writeArray(buf);

  // sysinfo
  tft.setTextSize(2);
  tft.drawRoundRect(5, 125, 310, 110, 10, ILI9341_CYAN);
  tft.drawLine(5, 125 + LINE_Y_REL, 314, 125 + LINE_Y_REL, ILI9341_CYAN);
  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.setCursor(55, 125 + TEXT_Y_REL);
  sprintf(buf, "System Information");
  writeArray(buf);
  tft.setCursor(15, 125 + FIRST_TEXT_Y_REL);
  sprintf(buf, "OS:");
  writeArray(buf);
  tft.setCursor(15, 125 + FIRST_TEXT_Y_REL + TEXT_ESPACE_SHORT_Y);
  sprintf(buf, "Kernel:");
  writeArray(buf);
  tft.setCursor(15, 125 + FIRST_TEXT_Y_REL + TEXT_ESPACE_SHORT_Y * 2);
  sprintf(buf, "Uptime:");
  writeArray(buf);

  gfx1_udpate();
}

void get_formated_space(int32_t valueMb) {
  int8_t unity = 0;
  if (valueMb > 999) {
    valueMb = valueMb / 1024;
    unity = 1;  // GB
  }

  if (valueMb > 9999) {
    valueMb = valueMb / 1024;
    unity = 2;  // TB
  }

  if (unity == 0)
    sprintf(buf, "%4d MB", valueMb);
  else if (unity == 1)
    sprintf(buf, "%4d GB", valueMb);
  else
    sprintf(buf, "%4d TB", valueMb);
}

static void gfx_update_mem() {
  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  if (s_ram_max != ramMax) {
    tft.setCursor(175 + 50, 25 + FIRST_TEXT_Y_REL);
    get_formated_space(ramMax);
    writeArray(buf);
    s_ram_max = ramMax;
  }

  if (s_ram_free != ramFree) {
    tft.setCursor(175 + 50, 25 + FIRST_TEXT_Y_REL + TEXT_ESPACE_SHORT_Y);
    get_formated_space(ramFree);
    writeArray(buf);
    s_ram_free = ramFree;
  }
}

static void gfx_update_cpu() {
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  if (cpuUsageGlobal != s_cpu_usage) {
    tft.setCursor(15 + 75, 25 + FIRST_TEXT_Y_REL);
    sprintf(buf, "%3d %%", cpuUsageGlobal);
    writeArray(buf);
    s_cpu_usage = cpuUsageGlobal;
  }
  if (s_cpuTemp != cpuTempGlobal) {
    tft.setCursor(15 + 75, 25 + FIRST_TEXT_Y_REL + TEXT_ESPACE_SHORT_Y);
    sprintf(buf, "%3d ", cpuTempGlobal);
    writeArray(buf);
    tft.setCursor(15 + 115, 25 + FIRST_TEXT_Y_REL + TEXT_ESPACE_SHORT_Y - 2);
    tft.setTextSize(1);
    tft.write('o');
    tft.setTextSize(2);
    tft.setCursor(15 + 115 + 8, 25 + FIRST_TEXT_Y_REL + TEXT_ESPACE_SHORT_Y);
    tft.write('C');
    s_cpuTemp = cpuTempGlobal;
  }
}

static void gfx_update_sys_info() {
  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);

  tft.setCursor(15 + 50, 125 + FIRST_TEXT_Y_REL);
  sprintf(buf, "%s", OS);
  writeArray(buf);
  s_os = OS;

  tft.setCursor(15 + 95, 125 + FIRST_TEXT_Y_REL + TEXT_ESPACE_SHORT_Y);
  sprintf(buf, "%s ", KernelVer);
  writeArray(buf);
  tft.setCursor(15 + 115, 125 + FIRST_TEXT_Y_REL + TEXT_ESPACE_SHORT_Y - 2);
  s_kernel = KernelVer;

  tft.setCursor(15 + 95, 125 + FIRST_TEXT_Y_REL + TEXT_ESPACE_SHORT_Y * 2);
  sprintf(buf, "%s ", Uptime);
  writeArray(buf);
  tft.setCursor(15 + 115, 125 + FIRST_TEXT_Y_REL + TEXT_ESPACE_SHORT_Y * 2 - 2);
  s_uptime = Uptime;
}

static void gfx_update_toolbar() {
  uint8_t c = 0;
  char buf[5];
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);

  if (s_time_tab[4] != time_tab[4] || s_time_tab[3] != time_tab[3]
      || s_time_tab[1] != time_tab[1] || s_time_tab[0] != time_tab[0]) {
    tft.setTextSize(2);
    tft.setCursor(135, 2);

    while (c < 5) {
      uint8_t car = time_tab[c];
      s_time_tab[c] = car;
      tft.write(car);
      c++;
    }
  }

  //temp
  if (s_localTemp != localTemp) {
    tft.setCursor(10, 2);
    if (localTemp != 255)
      sprintf(buf, "%2d", localTemp);
    else
      sprintf(buf, "--");
    writeArray(buf);
    s_localTemp = localTemp;
  }

  // humidity
  if (s_localHum != localHum) {
    tft.setCursor(275, 2);
    if (localHum != 255)
      sprintf(buf, "%2d", localHum);
    else
      sprintf(buf, "--");
    writeArray(buf);
    s_localHum = localHum;
  }
}
