//Includes and Setup
#include "mbed.h"
#include "Temp&Humidity.h"
#include "Motors.h"
#include "CO2.h"
#include "LDR.h"
#include <iostream>
#include <chrono>
using namespace std;
static BufferedSerial serial_port(USBTX, USBRX);

Dht11 DHT11pin(D2);


//Class defenitions
Motors Motor;
CO2 CO2;
LDR LDR;

//Thread defenitions
Thread Thread_Motor;
Thread Thread_DHT11;
Thread Thread_CO2;
Thread Thread_LDR;

//EventQueue setup
EventQueue Queue_Motor(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_DHT11(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_CO2(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_LDR(1 * EVENTS_EVENT_SIZE);

//Functions
void Motors(){
    //Motor.LineFollowing();
    Motor.motorsForward();
}

void DHT11(){
    DHT11pin.readDHT11();
    int TEMP = DHT11pin.getCelsius();
    //int TEMP = DHT11.getFahrenheit();
    printf("T: %d, H: %d\r\n", TEMP, DHT11pin.getHumidity());
}

void CO2read(){
    CO2.ReadCO2();
    CO2.CalculatePartsPerMinute();
    printf("Environmental Sensor Value %d\n\r", CO2.ppm);
    //wait_us(1000000);  
}

void LDRread(){
    LDR.ReadLDR();
    printf("LDR: %d \n", LDR.LDR);
    //wait_us(1000);
}




int main(){
    // Set up the serial port to Baud 115200
    serial_port.set_baud(115200);

    Motor.motorSetup();
    Queue_Motor.call_every(1s, Motors);
    Queue_DHT11.call_every(1s, DHT11);
    Queue_CO2.call_every(1s, CO2read);
    Queue_LDR.call_every(1s, LDRread);

    //Thread_Motor.start(callback(&Queue_Motor, &EventQueue::dispatch_forever));
    Thread_DHT11.start(callback(&Queue_DHT11, &EventQueue::dispatch_forever));
    Thread_CO2.start(callback(&Queue_CO2, &EventQueue::dispatch_forever));
    Thread_LDR.start(callback(&Queue_LDR, &EventQueue::dispatch_forever));
}