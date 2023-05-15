import socket
import sys
import network
import time
from time import sleep
from picozero import pico_temp_sensor, pico_led

time.sleep(3)
pico_led.on()
print('Hello World!')
uart = machine.UART(1, 9600, tx=machine.Pin(8), rx = machine.Pin(9))
uart.init(9600, bits=8, parity=None, stop=1)
print(uart)


w = network.WLAN()
w.active(True)
w.connect('VM7267268','xtj5fbkmKbLx')
w.ifconfig()


temperatureC = 0
temperatureF = 0
humidity = 0
CO2 = 0
light = 0
i = 0

sock = socket.socket()
while True:
    if (i == 10):
        machine.reset()
    else:
        try:
            sock.connect(('192.168.0.54', 8888))
        except Exception:
            print('Cannot Connect')
            i+=1
            time.sleep(1)
            continue
        break
    
print('Connected')

while True:
    msg = ""
    if uart.any():
        b = uart.readline()
        msg = b.decode('utf-8')
        print(msg)
        sock.write(str(msg))

