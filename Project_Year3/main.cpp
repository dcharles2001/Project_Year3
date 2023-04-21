//Includes and Setup
#include "mbed.h"
#include "Temp&Humidity.h"
#include "Motors.h"
#include "CO2.h"
#include "LDR.h"
#include <iostream>
#include <chrono>
#include <string>

using namespace std;

static BufferedSerial serial_port(USBTX, USBRX);
static BufferedSerial UART(PE_8, PE_7);


Timer tick;
DigitalOut myservo(D0);
//Class defenitions
Motors Motor;
CO2 CO2;
LDR LDR;
Dht11 DHT11;

//Thread defenitions
Thread Thread_COMM;
Thread Thread_Motor;
Thread Thread_DHT11;
Thread Thread_CO2;
Thread Thread_LDR;

//EventQueue setup
EventQueue Queue_COMM(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_Motor(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_DHT11(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_CO2(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_LDR(1 * EVENTS_EVENT_SIZE);

string C;
string F;
string H;
string SEND1;
string SEND2;
string SEND3;
string GAS;
string LIGHT;

void COMM(){
    SEND1 = "-"+C+"-"+F+"\r\n";
    SEND2 = "-"+H+"-"+GAS+"\r\n";
    SEND3 = "-"+LIGHT+"\r\n";

    UART.write(SEND1.c_str(),sizeof(SEND1));
    //wait_us(1500);
    UART.write(SEND2.c_str(),sizeof(SEND2));
    //wait_us(1500);
    UART.write(SEND3.c_str(),sizeof(SEND3));
    int d = tick.read();
    printf("%d\n",d);
    if (d>50){
        myservo = 1;
        wait_us(1000000);
        myservo = 0;
        wait_us(20000000 - 1000000);
        printf("Reset timer\n");
        tick.reset();
    }
}
//Functions
void Motors(){                                                  // Read the Line senors and move the motors to match
    Motor.LineFollowing();
    //Motor.motorsForward();
}

void DHT11read(){  
    DHT11.DHT11setup();                                         // Read the Temp/Humidity sesnor
    DHT11.readDHT11();
    printf("C: %d, F: %d, H: %d\n\n", DHT11.getCelsius(), DHT11.getFahrenheit(), DHT11.getHumidity());
    C = to_string(DHT11.getCelsius());
    H = to_string(DHT11.getHumidity());
    F = to_string(DHT11.getFahrenheit());
}

void CO2read(){                                                 // Read the Environemntal Sensor and work out PPM
    CO2.ReadCO2();
    CO2.CalculatePartsPerMinute();
    printf("Environmental Sensor Value %d\n",CO2.ppm);
    GAS = to_string(0);//CO2.ppm);
}

void LDRread(){                                                 // Read the LDR                                  
    LDR.ReadLDR();
    printf("LDR: %d \n", LDR.LDR);
    LIGHT = to_string(LDR.LDR);
}


int main(){
    tick.start();
    serial_port.set_baud(115200);                               // Set up the serial port to Baud 115200
    serial_port.set_format(
        /* bits */ 8,
        /* parity */ BufferedSerial::None,
        /* stop bit */ 1
    );

    UART.set_baud(9600);
    UART.set_format(
       /* bits */ 8,
        /* parity */ BufferedSerial::None,
        /* stop bit */ 1
    );
    
    Motor.motorSetup();                                         // Set up motor drivers

    Queue_Motor.call_every(100ms, Motors);                      // Call the motor function every 100ms
    Queue_DHT11.call_every(1s, DHT11read);                      // Call the Temp/Humidity sensor every 1s
    Queue_CO2.call_every(1s, CO2read);                          // Call the Environmental sensor every 1s
    Queue_LDR.call_every(1s, LDRread);                          // Call the LDR every 1s
    Queue_COMM.call_every(10s, COMM);                            // Call the COMM function every 1s

    Thread_Motor.start(callback(&Queue_Motor, &EventQueue::dispatch_forever));      // Start thread for the motors
    Thread_DHT11.start(callback(&Queue_DHT11, &EventQueue::dispatch_forever));      // Start thread for the Temp/Humidity Sensor
    Thread_CO2.start(callback(&Queue_CO2, &EventQueue::dispatch_forever));          // Start thread for the Environmental Sensor
    Thread_LDR.start(callback(&Queue_LDR, &EventQueue::dispatch_forever));          // Start thread for the LDR
    Thread_COMM.start(callback(&Queue_COMM, &EventQueue::dispatch_forever));
}