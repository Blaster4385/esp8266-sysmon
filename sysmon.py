#!/usr/bin/python

import sys
from datetime import datetime
from datetime import timedelta
import psutil
import serial
from time import sleep
import sensors
import subprocess
import math
import os

CPU_TEMP_PATH = '/sys/devices/pci0000:00/0000:00:18.3/hwmon/hwmon2/temp1_input'

file_descriptor = '/dev/ttyUSB0'
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
    # request = 'RamMax=%d\r\n' % ram_max
    # send_command(request)
    return ram_max

def set_free():
    ram_free = int(psutil.virtual_memory().free / (1024.*1024.))
    # request = 'RamFree=%d\r\n' % ram_free
    # send_command(request)
    return ram_free

def set_OS():
    with open('/etc/os-release', 'r') as f:
        for line in f:
            if line.startswith('NAME='):
                OSName = line.split('"')[1]
                break
    return OSName

def set_kernel_version():
    with open('/proc/version', 'r') as f:
        for line in f:
            Kernel = line.split(' ')[2].split('-')[0]
            break
    return Kernel

def set_uptime():
    uptime_seconds = psutil.boot_time()
    uptime = datetime.now() - datetime.fromtimestamp(uptime_seconds)
    uptime_hours = int(uptime.total_seconds() // 3600)
    uptime_minutes = int((uptime.total_seconds() % 3600) // 60)
    uptime = f"{uptime_hours:02d}:{uptime_minutes:02d}"
    return uptime

def start():
    while 1:
        cpu_temp = int(subprocess.check_output(['cat', CPU_TEMP_PATH]))/1000
        cpu_pct = psutil.cpu_percent(interval=None, percpu=False)
        cpu_global = int(cpu_pct)
        request = "\nCpuTemp="+str(math.trunc(cpu_temp))+",CpuUsage="+str(math.trunc(cpu_global))+",RamMax="+str(set_max())+",RamFree="+str(set_free())+",Time="+str(set_time(datetime.now()))+",OS="+str(set_OS())+",Kernel="+str(set_kernel_version())+",Uptime="+str(set_uptime())
        send_command(request)
        sleep(2)


def main():
    start()

if __name__ == "__main__":
    main()
