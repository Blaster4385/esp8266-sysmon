#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <XPT2046_Touchscreen.h>
#include "DHT.h"
#include "cpu.h"
#include "datetimeHandler.h"
#include "gfx_1.h"

#define TFT_DC D4
#define TFT_CS D2
#define TFT_RESET D3
#define DHTPIN D8
#define DHTTYPE DHT22

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
DHT dht(DHTPIN, DHTTYPE);

static uint32_t tempTime = 0;
static uint32_t rtcTime = 0;
static uint32_t displayTime = 0;

void setup() {
  Serial.begin(9600);
  initHardware();
  gfx1_layout();
}

void update(String command) {
  if (!isnan(dht.readTemperature())) {
    localHum = dht.readHumidity();
    localTemp = dht.readTemperature() - 4;
  }

  String keys[] = { "CpuTemp", "CpuUsage", "RamMax", "RamFree", "Time", "OS", "Kernel", "Uptime" };
  String values[] = { "", "", "", "", "", "", "", "" };

  for (int i = 0; i < std::size(keys); i++) {

    int index = command.indexOf(keys[i]);

    if (index != -1) {
      String value = command.substring(index + keys[i].length() + 1, command.indexOf(",", index + keys[i].length() + 1));
      values[i] = value;
    }
  }

  for (int i = 0; i < std::size(values); i++) {
    Serial.print(keys[i]);
    Serial.print(" = ");
    Serial.println(values[i]);
    switch (i) {
      case 0:
        if (values[i] != "")
          setCpuTemp(values[i]);
        break;
      case 1:
        if (values[i] != "")
          setCpuUsage(values[i]);
        break;
      case 2:
        if (values[i] != "")
          setRamMax(values[i]);
        break;
      case 3:
        if (values[i] != "")
          setRamFree(values[i]);
        break;
      case 4:
        if (values[i] != "")
          setTime(values[i]);
        break;
      case 5:
        if (values[i] != "")
          OS = values[i];
        break;
      case 6:
        if (values[i] != "")
          KernelVer = values[i];
        break;
      case 7:
        if (values[i] != "")
          Uptime = values[i];
        break;
    }
  }
  gfx1_udpate();
}

void loop() {
  String command = Serial.readString();
  command.trim();
  update(command);
}

void initHardware() {
  pinMode(TFT_RESET, OUTPUT);
  digitalWrite(TFT_RESET, LOW);
  delay(1000);
  digitalWrite(TFT_RESET, HIGH);
  delay(1000);
  tft.begin();
  tft.setRotation(3);
  dht.begin();
}