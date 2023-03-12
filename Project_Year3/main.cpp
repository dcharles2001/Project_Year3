//Includes and Setup
#include "mbed.h"
#include "Temp_Humidity.h"
#include "Motors.h"
#include "CO2.h"
#include "LDR.h"
#include "DataSend.h"
#include <iostream>
#include <chrono>
using namespace std;
static BufferedSerial serial_port(USBTX, USBRX);

int TEMP;

//Class defenitions
Motors Motor;
CO2 CO2;
LDR LDR;
Temp_Humidity Temp_Humidity;

//Thread defenitions
Thread Thread_Motor;
Thread Thread_Temp_Humidity;
Thread Thread_CO2;
Thread Thread_LDR;

//EventQueue setup
EventQueue Queue_Motor(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_Temp_Humidity(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_CO2(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_LDR(1 * EVENTS_EVENT_SIZE);

//Functions
void Motors(){
    Motor.LineFollowing();
    //Motor.motorsForward();
}

void Temp_Humidityread(){  
    Temp_Humidity.DHT11setup();                                                 // Read the Temp/Humidity sesnor
    Temp_Humidity.readDHT11();
    TEMP = Temp_Humidity.getCelsius();
    //TEMP = DHT11.getFahrenheit();
    printf("T: %d, H: %d\n\n", TEMP, Temp_Humidity.getHumidity());
}

void CO2read(){
    CO2.ReadCO2();
    CO2.CalculatePartsPerMinute();
    printf("Environmental Sensor Value %d\n", CO2.ppm); 
}

void LDRread(){
    LDR.ReadLDR();
    printf("LDR: %d \n", LDR.LDR);
}




int main(){
    // Set up the serial port to Baud 115200
    serial_port.set_baud(115200);

    Motor.motorSetup();
    Queue_Motor.call_every(1s, Motors);
    Queue_Temp_Humidity.call_every(1s, Temp_Humidity);
    Queue_CO2.call_every(1s, CO2read);
    Queue_LDR.call_every(1s, LDRread);

    Thread_Motor.start(callback(&Queue_Motor, &EventQueue::dispatch_forever));
    Thread_Temp_Humidity.start(callback(&Queue_Temp_Humidity, &EventQueue::dispatch_forever));
    Thread_CO2.start(callback(&Queue_CO2, &EventQueue::dispatch_forever));
    Thread_LDR.start(callback(&Queue_LDR, &EventQueue::dispatch_forever));
}