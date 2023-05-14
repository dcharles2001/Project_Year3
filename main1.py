# receiver.py / Tx/Rx => Tx/Rx
import os
import machine
import network
import socket
import time
from time import sleep
from picozero import pico_temp_sensor, pico_led


ssid = 'VM7267268'
password = 'xtj5fbkmKbLx'
#ssid = 'You Dont Need This'
#password = 'razpipicow'

uart = machine.UART(1, 9600, tx=machine.Pin(8), rx = machine.Pin(9))
uart.init(9600, bits=8, parity=None, stop=1)
print(uart)

b = None
msg = ""

def connect():
    #Connect to WLAN
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(ssid, password)
    timer = time.time()
    while wlan.isconnected() == False:
        print('Waiting for connection...')
        seconds = time.time()
        timeout = seconds - timer
        print(timeout)
        if timeout > 10:
            machine.reset()
        else:
            sleep(1)
    ip = wlan.ifconfig()[0]
    print(f'Connected on {ip}')
    return ip

def open_socket(ip):
    # Open a socket
    address = (ip, 8888)
    connection = socket.socket()
    connection.bind(address)
    connection.listen(1)
    return connection
'''    
def webpage(temperatureC, temperatureF, humidity, CO2, light, state):
    #Template HTML
    html = f"""
            <!DOCTYPE html>
            <meta http-equiv="refresh" content="10" >
            <html>
            <form action="./lighton">
            <input type="submit" value="Light on" />
            </form>
            <form action="./lightoff">
            <input type="submit" value="Light off" />
            </form>
            <p>LED is {state}</p>
            <p>The Temperature is {temperatureC}Celsius</p>
            <p>The Temperature is {temperatureF}Fahrenheit</p>
            <p>The Humidity is {humidity}g/Kg</p>
            <p>The CO2 levels are {CO2}ppm</p>
            <p>The Light value is {light}</p>
            </body>
            </html>
            """
    return str(html)

def serve(connection):
    #Start a web server
    state = 'ON'
    pico_led.on()
    temperatureC = 0
    temperatureF = 0
    humidity = 0
    CO2 = 0
    light = 0
    i = 0
    timer = time.time()
    while True:
        client = connection.accept()[0]
        request = client.recv(1024)
        request = str(request)
        
        seconds = time.time()
        timeout = seconds - timer
        print(timeout)
        if timeout > 20:
            machine.reset()
        else:
            try:
                request = request.split()[1]
            except IndexError:
                pass
            if uart.any():
                b = uart.readline()
                msg  = b.decode('utf-8')
                print(msg)
                timer = time.time()
                messagelist = msg.split("-")
                if i == 0:
                    temperatureC = messagelist[1]
                    temperatureF = messagelist[2]
                    i = 1
                elif i == 1:
                    humidity = messagelist[1]
                    CO2 = messagelist[2]
                    i = 2
                elif i == 2:
                    light = messagelist[1]
                    i = 0
                
        if request =='/lightoff?':
            pico_led.off()
            state = 'OFF'
        elif request == '/lighton?':
            pico_led.on()
            state = 'ON'
            
        html = webpage(temperatureC, temperatureF, humidity, CO2, light, state)
        client.send(html)
        
        client.close()
'''

def serve(connection):
    client = connection.accept()[0]
    #request = client.recv(1024)
    client.send("Hello World!")
    pico_led.on()
    client.close()
   

try:
    ip = connect()
    connection = open_socket(ip)
    serve(connection)
except KeyboardInterrupt:
    machine.reset()