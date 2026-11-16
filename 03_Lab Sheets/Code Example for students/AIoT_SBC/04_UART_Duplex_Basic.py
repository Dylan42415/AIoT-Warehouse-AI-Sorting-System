#! /usr/bin/python3
#import time library
import time
import serial

serial1 = serial.Serial('/dev/ttyUSB0',9600)

while True:
  dataString = serial1.readline()
  print(dataString)
  time.sleep(3)
  serial1.write("Hello Arduino\n\r".encode())
ser.close()#end of while loop, close the serial interface
