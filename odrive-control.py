#!/usr/bin/env python3
"""
Example usage of the ODrive python library to monitor and control ODrive devices
"""

from __future__ import print_function
"""
import odrive
from odrive.enums import *
from odrive.utils import *
import time
import math
"""
import serial
ser = serial.Serial('/dev/ttyACM1', 115200)

"""
# Find a connected ODrive (this will block until you connect one)
print("finding an odrive...")
my_drive = odrive.find_any()

# Find an ODrive that is connected on the serial port /dev/ttyUSB0
#my_drive = odrive.find_any("serial:/dev/ttyUSB0")

dump_errors(my_drive, True)

# Calibrate motor and wait for it to finish
print("starting calibration...")

my_drive.axis1.requested_state = AXIS_STATE_FULL_CALIBRATION_SEQUENCE
while my_drive.axis1.current_state != AXIS_STATE_IDLE:
    time.sleep(0.1)
my_drive.axis1.requested_state = AXIS_STATE_CLOSED_LOOP_CONTROL

# time.sleep(5)
# my_drive.axis1.controller.pos_setpoint = -4000
# while 1:
  #  print(my_drive.axis1.encoder.pos_estimate)

# time.sleep(5)

"""

bytecounter = 0

print("reading emg signal")
while 1:
    if(ser.in_waiting > 0):

        emgSig = ser.readline()
        """
        for n in emgSig:
            try:
                temp = int(n)
                if(temp < 200):
                    digit = temp
                else:
                    break
                print(digit)
            except:
                break
        """
        print(emgSig)


# velocity = 500


# if(emgSig > 20):
  #  my_drive.axis1.controller.pos_setpoint += velocity
   # else:
    #    my_drive.axis1.controller.pos_setpoint -= velocity
"""
print("failed reading emg signal")



# A sine wave to test
t0 = time.monotonic()
while True:
    setpoint = 10000.0 * math.sin((time.monotonic() - t0)*2)
    print("goto " + str(int(setpoint)))
    my_drive.axis1.controller.pos_setpoint = setpoint
    time.sleep(0.01)



while my_drive.axis1.current_state != AXIS_STATE_IDLE:
    time.sleep(0.1)

my_drive.axis1.requested_state = AXIS_STATE_CLOSED_LOOP_CONTROL

import serial
ser = serial.Serial('COM6', 9600)
while 1:
    if(ser.in_waiting > 0):
        line = ser.readline()
        print(line)

# To read a value, simply read the property
print("Bus voltage is " + str(my_drive.vbus_voltage) + "V")


# Or to change a value, just assign to the property
my_drive.axis1.controller.pos_setpoint = 3.14
print("Position setpoint is " + str(my_drive.axis1.controller.pos_setpoint))

# And this is how function calls are done:
for i in [1,2,3,4]:
    print('voltage on GPIO{} is {} Volt'.format(i, my_drive.get_adc_voltage(i)))

# A sine wave to test
t0 = time.monotonic()
while True:
    setpoint = 10000.0 * math.sin((time.monotonic() - t0)*2)
    print("goto " + str(int(setpoint)))
    my_drive.axis1.controller.pos_setpoint = setpoint
    time.sleep(0.01)
"""

# Some more things you can try:
"""
# Write to a read-only property:
my_drive.vbus_voltage = 11.0  # fails with `AttributeError: can't set attribute`

# Assign an incompatible value:
my_drive.motor0.pos_setpoint = "I like trains"  # fails with `ValueError: could not convert string to float`
"""