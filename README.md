# ESP8266 Based System Monitor

## Disclaimer!

### This project is based on work from [execuc/esp-hw-monitoring](https://github.com/execuc/esp-hw-monitoring) with several modifications to fit my needs. I take no credit for the original work.

### Modifications include but are not limited to:

- Major code cleanup
- Data format modification
- More efficient data reading

---

## What is this?

This is a simple ESP8266 based system monitor that can be used to monitor system information and resources. It also shows the current time as well as the ambient temperature and humidity.

## Components used

- ESP8266
- 240*320 2.8 inch ILI9341 LCD 
- DHT22 temperature module (with resistor)

## Connecting the components

- Connect the LCD to the ESP8266 as follows:
    - LCD VCC to ESP8266 3.3V
    - LCD GND to ESP8266 GND
    - LCD CS to ESP8266 D2
    - LCD RESET to ESP8266 D3
    - LCD DC to ESP8266 D4

- Connect the DHT22 to the ESP8266 as follows:
    - DHT22 VCC to ESP8266 3.3V
    - DHT22 GND to ESP8266 GND
    - DHT22 DATA to ESP8266 D8

## How to use

- Install the Arduino IDE
- Install the ESP8266 board
- Install the following libraries:
    - Adafruit GFX Library
    - Adafruit ILI9341
    - Adafruit Unified Sensor
    - DHT sensor library

- Open the `sysmon.ino` file in the Arduino IDE

- Upload the code to the ESP8266

- Run the sysmon.py script on your computer

- Enjoy!


## Note for Windows users

Download [Open Hardware Monitor](https://openhardwaremonitor.org/downloads/) and put the OpenHardwareMonitorLib.dll file alongside the sysmon_windows.py file.
