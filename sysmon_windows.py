#!/usr/bin/python

import sys
from datetime import datetime
from datetime import timedelta
import psutil
import serial
from time import sleep
import math
import platform
import clr
clr.AddReference(r'OpenHardwareMonitorLib')
from OpenHardwareMonitor import Hardware

file_descriptor = 'COM5'
baud_rate = 9600

arduino = serial.Serial(port=file_descriptor, baudrate=baud_rate)
sleep(6)

def send_command(command):
    print("sent: " + command)
    arduino.write(command.encode())
    sleep(0.1)

def set_time(date_time):
    curr_time=date_time.strftime('%H:%M:%S')
    return curr_time


def set_max():
    ram_max = int(psutil.virtual_memory().total / (1024.*1024.))
    return ram_max

def set_free():
    ram_free = int(psutil.virtual_memory().free / (1024.*1024.))
    return ram_free

def set_OS():
    OSName = platform.uname()[0]
    return OSName

def set_kernel_version():
    Kernel = platform.uname().version
    return Kernel

def set_uptime():
    uptime_seconds = psutil.boot_time()
    uptime = datetime.now() - datetime.fromtimestamp(uptime_seconds)
    uptime_hours = int(uptime.total_seconds() // 3600)
    uptime_minutes = int((uptime.total_seconds() % 3600) // 60)
    uptime = f"{uptime_hours:02d}:{uptime_minutes:02d}"
    return uptime

def get_cpu_temperature():
    computer = Hardware.Computer()
    computer.Open()
    computer.CPUEnabled = True
    
    for hardware in computer.Hardware:
        if hardware.HardwareType == Hardware.HardwareType.CPU:
            hardware.Update()
            for sensor in hardware.Sensors:
                if sensor.SensorType == Hardware.SensorType.Temperature and sensor.Name == "CPU Package":
                    return sensor.Value

def start():
    while 1:
        cpu_temp = get_cpu_temperature()
        cpu_pct = psutil.cpu_percent(interval=None, percpu=False)
        cpu_global = int(cpu_pct)
        request = "\nCpuTemp="+str(cpu_temp)+",CpuUsage="+str(math.trunc(cpu_global))+",RamMax="+str(set_max())+",RamFree="+str(set_free())+",Time="+str(set_time(datetime.now()))+",OS="+str(set_OS())+",Kernel="+str(set_kernel_version())+",Uptime="+str(set_uptime())
        send_command(request)
        sleep(2)


def main():
    start()

if __name__ == "__main__":
    main()