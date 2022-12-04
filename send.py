import time
import argparse
import signal
import sys
import socket
import socketserver
import serial
import threading

SERIALPORT = "/dev/pts/3"
BAUDRATE = 115200
ser = serial.Serial()


def initUART():
    # ser = serial.Serial(SERIALPORT, BAUDRATE)
        ser.port=SERIALPORT
        ser.baudrate=BAUDRATE
        ser.bytesize = serial.EIGHTBITS #number of bits per bytes
        ser.parity = serial.PARITY_NONE #set parity check: no parity
        ser.stopbits = serial.STOPBITS_ONE #number of stop bits
        ser.timeout = None          #block read

        # ser.timeout = 0             #non-block read
        # ser.timeout = 2              #timeout block read
        ser.xonxoff = False     #disable software flow control
        ser.rtscts = False     #disable hardware (RTS/CTS) flow control
        ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
        #ser.writeTimeout = 0     #timeout for write
        print("Starting Up Serial Monitor")
        try:
                ser.open()
        except serial.SerialException:
                print("Serial {} port not available".format(SERIALPORT))
                exit()

def sendUARTMessage(msg):
    ser.write(msg)

if __name__ == '__main__':
        initUART()
        temp = 0
        hum = 0
        
        while True:
                temp = temp + 1
                hum = hum + 1
                message = ("temp: " + str(temp) +";"+" hum: " + str(hum)+";").encode()
                sendUARTMessage(message)
                time.sleep(1)
    

