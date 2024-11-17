#!/usr/bin/python

import sys
import psutil
import serial
import platform
from time import sleep, time
from datetime import datetime
import os

if platform.system() == "Windows":
    import win32com.client

# Constants
FILE_DESCRIPTOR = 'COM3' if platform.system() == 'Windows' else '/dev/ttyUSB0'
BAUD_RATE = 115200
SERIAL_TIMEOUT = 1  # Timeout for serial operations in seconds
CONNECTION_WAIT = 6  # Time to wait for the serial connection to establish
SEND_INTERVAL = 3  # Interval between sending data in seconds

# CPU temperature path for Linux
#CPU_TEMP_PATH = '/sys/devices/pci0000:00/0000:00:18.3/hwmon/hwmon4/temp1_input'
CPU_TEMP_PATH = os.environ.get('CPU_TEMP_PATH', '/sys/devices/pci0000:00/0000:00:18.3/hwmon/hwmon3/temp1_input')

# Open serial connection
def open_serial_connection():
    try:
        return serial.Serial(port=FILE_DESCRIPTOR, baudrate=BAUD_RATE, timeout=SERIAL_TIMEOUT)
    except serial.SerialException as e:
        sys.exit(f"Failed to connect to the serial port: {e}")


# Send command to Arduino
def send_command(arduino, command):
    try:
        arduino.write(command.encode())
        sleep(0.1)
    except serial.SerialException as e:
        print(f"Failed to send command: {e}")


# Get the current time
def get_time():
    return datetime.now().strftime('%H:%M:%S')


# Get RAM information
def get_ram_info():
    ram = psutil.virtual_memory()
    return int(ram.total / (1024 * 1024)), int(ram.available / (1024 * 1024))


# Get OS name
def get_os_name():
    if platform.system() == 'Windows':
        return platform.system()
    else:
        try:
            with open('/etc/os-release') as f:
                for line in f:
                    if line.startswith('NAME='):
                        return line.split('=')[1].strip().strip('"')
        except FileNotFoundError:
            return "Unknown"


# Get kernel version
def get_kernel_version():
    if platform.system() == 'Windows':
        return platform.version()
    else:
        try:
            with open('/proc/version') as f:
                return f.readline().split()[2].split('-')[0]
        except FileNotFoundError:
            return "Unknown"


# Get system uptime
def get_uptime():
    uptime_seconds = psutil.boot_time()
    uptime = datetime.now() - datetime.fromtimestamp(uptime_seconds)
    hours, remainder = divmod(int(uptime.total_seconds()), 3600)
    minutes = remainder // 60
    return f"{hours:02d}:{minutes:02d}"


# Get CPU temperature
def get_cpu_temp():
    if platform.system() == 'Windows':
        # For Windows, use WMI to get CPU temperature
        wmi = win32com.client.Dispatch("WbemScripting.SWbemLocator")
        service = wmi.ConnectServer(".", "root\cimv2")
        temperature_info = service.ExecQuery(
            "SELECT * FROM Win32_TemperatureProbe")
        for sensor in temperature_info:
            if sensor.CurrentReading is not None:
                # Convert from tenths of Kelvin to Celsius
                return sensor.CurrentReading / 10.0 - 273.15
        return 0  # Return 0 if there's no temperature sensor data
    else:
        try:
            with open(CPU_TEMP_PATH) as f:
                return int(f.read().strip()) / 1000
        except (FileNotFoundError, ValueError):
            return 0  # Return 0 if there's an error reading CPU temperature


# Main data gathering and sending function
def gather_and_send_data(arduino, ram_max, os_name, kernel_version):
    while True:
        start_time = time()
        try:
            cpu_temp = int(get_cpu_temp())
            cpu_usage = int(psutil.cpu_percent(interval=None))
            _, ram_free = get_ram_info()
            current_time = get_time()
            uptime = get_uptime()

            request = (f"\nCpuTemp={cpu_temp},CpuUsage={cpu_usage},RamMax={ram_max},"
                       f"RamFree={ram_free},Time={current_time},OS={os_name},"
                       f"Kernel={kernel_version},Uptime={uptime}")

            send_command(arduino, request)
        except Exception as e:
            print(f"An error occurred: {e}")

        elapsed_time = time() - start_time
        if elapsed_time < SEND_INTERVAL:
            sleep(SEND_INTERVAL - elapsed_time)


# Main function to handle program start
def main():
    arduino = open_serial_connection()
    sleep(CONNECTION_WAIT)  # Wait for the connection to establish

    ram_max, _ = get_ram_info()
    os_name = get_os_name()
    kernel_version = get_kernel_version()

    try:
        gather_and_send_data(arduino, ram_max, os_name, kernel_version)
    except KeyboardInterrupt:
        print("Interrupted by user")
    finally:
        if arduino.is_open:
            arduino.close()


if __name__ == "__main__":
    main()
