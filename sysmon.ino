#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <XPT2046_Touchscreen.h>
#include "DHT.h"
#include "cpu.h"
#include "datetimeHandler.h"
#include "gfx_1.h"

#define TFT_DC  D4
#define TFT_CS D2
#define TFT_RESET D3
#define DHTPIN D8
#define DHTTYPE DHT22 

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
DHT dht(DHTPIN, DHTTYPE);

static uint32_t tempTime  = 0; 
static uint32_t rtcTime  = 0;
static uint32_t displayTime = 0;

void setup() {
  Serial.begin(9600);
  initHardware();
  gfx1_layout();
}

void update(String command) {
      int delimiterIndex = command.indexOf('=');
      // Serial.println(command);
    if (delimiterIndex != -1) {
      String key = command.substring(0, delimiterIndex);
      String value = command.substring(delimiterIndex + 1);
      Serial.print(key);

      // Check the key and assign the corresponding value
      if (key.equals("CpuTemp")) {
        setCpuTemp(value);
        Serial.print("Received CPU temperature: ");
        Serial.println(value);
        // gfx1_udpate();
      } else if (key.equals("CpuUsage")) {
        setCpuUsage(value);
        Serial.print("Received CPU usage: ");
        Serial.println(value);
        // gfx1_udpate();
      } else if (key.equals("RamFree")) {
        setRamFree(value);
        Serial.print("Received Ram free: ");
        Serial.println(value);
        // gfx1_udpate();
      } else if (key.equals("RamMax")) {
        setRamMax(value);
        Serial.print("Received Ram max: ");
        Serial.println(value);
        // gfx1_udpate();
      } else if (key.equals("Time")) {
        setTime(value);
        Serial.print("Received Time: ");
        Serial.println(value);
        // gfx1_udpate();
      }
    }
    gfx1_refresh = true;
    gfx1_udpate();
}

void loop() {
  Serial.println("Enter data:");
  // while (Serial.available() == 0) {}   
  String command = Serial.readString();
  command.trim();                       
  Serial.println(command); 

  if(!isnan(dht.readTemperature()))
    {
      localHum = dht.readHumidity();
      localTemp = dht.readTemperature() - 4;
      Serial.println(localTemp);
      gfx1_refresh = true;
      gfx1_udpate();
    }
  // else
  //   {
  //     localHum = 255;
  //     localTemp = 255;
  //   }

  update(command);
  // delay(1000);

  // if ( currentTime > displayTime ) {
  //   displayTime = currentTime + 100000;
  // }
}

void initHardware()
{
  pinMode(TFT_RESET, OUTPUT);
  digitalWrite(TFT_RESET, LOW);
  delay(1000);
  digitalWrite(TFT_RESET, HIGH);
  delay(1000);
  tft.begin();
  tft.setRotation(3);
  dht.begin();
  // delay(3000);
}