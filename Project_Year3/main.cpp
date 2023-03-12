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
Temp_Humidity DHT11;

//Thread defenitions
Thread Thread_Motor;
Thread Thread_DHT11;
Thread Thread_CO2;
Thread Thread_LDR;
Thread Thread_DataSend;

//EventQueue setup
EventQueue Queue_Motor(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_DHT11(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_CO2(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_LDR(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_DataSend(1 * EVENTS_EVENT_SIZE);

//Functions
void Motors(){                                                  // Read the Line senors and move the motors to match
    Motor.LineFollowing();
    //Motor.motorsForward();
}

void DHT11read(){  
    DHT11.DHT11setup();                                                 // Read the Temp/Humidity sesnor
    DHT11.readDHT11();
    TEMP = DHT11.getCelsius();
    //TEMP = DHT11.getFahrenheit();
    //printf("T: %d, H: %d\n\n", TEMP, DHT11.getHumidity());
}

void CO2read(){                                                 // Read the Environemntal Sensor and work out PPM
    CO2.ReadCO2();
    CO2.CalculatePartsPerMinute();
    //printf("Environmental Sensor Value %d\n", CO2.ppm); 
}

void LDRread(){                                                 // Read the LDR                                  
    LDR.ReadLDR();
    //printf("LDR: %d \n", LDR.LDR);
}

void DataSend(){
    printf("T: %d, H: %d\n", TEMP, DHT11.getHumidity());
    printf("Environmental Sensor Value %d\n", CO2.ppm);
    printf("LDR: %d \n\n", LDR.LDR);
}


int main(){
    serial_port.set_baud(115200);                               // Set up the serial port to Baud 115200

    Motor.motorSetup();                                         // Set up motor drivers

    Queue_Motor.call_every(100ms, Motors);                      // Call the motor function every 100ms
    Queue_DHT11.call_every(1s, DHT11read);                      // Call the Temp/Humidity sensor every 1s
    Queue_CO2.call_every(1s, CO2read);                          // Call the Environmental sensor every 1s
    Queue_LDR.call_every(1s, LDRread);                          // Call the LDR function every 1s
    Queue_DataSend.call_every(1s, DataSend);                    // Call the DataSend function every 1s

    Thread_Motor.start(callback(&Queue_Motor, &EventQueue::dispatch_forever));      // Start thread for the motors
    Thread_DHT11.start(callback(&Queue_DHT11, &EventQueue::dispatch_forever));      // Start thread for the Temp/Humidity Sensor
    Thread_CO2.start(callback(&Queue_CO2, &EventQueue::dispatch_forever));          // Start thread for the Environmental Sensor
    Thread_LDR.start(callback(&Queue_LDR, &EventQueue::dispatch_forever));          // Start thread for LDR
    Thread_DataSend.start(callback(&Queue_DataSend, &EventQueue::dispatch_forever));          // Start thread for DataSending
}