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


//Thread defenitions
Thread Thread_Motor;
Thread Thread_DHT11;
Thread Thread_CO2;
Thread Thread_LDR;

EventQueue Queue_Motor(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_DHT11(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_CO2(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_LDR(1 * EVENTS_EVENT_SIZE);

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

uint16_t adc_rd;
uint16_t ppm;
AnalogIn Co2(A0);
void CalculatePPM()
{
    const double Rl      = 5000.0;               // Rl (Ohm) - Load resistance
    const double Vadc_33 = 0.0000503548;         // ADC step 3,3V/65535 0.00503mV (16bit ADC)
    double Vrl;                                  // Output voltage
    double Rs;                                   // Rs (Ohm) - Sensor resistance
    double ratio;                                // Rs/Rl ratio
    double lgPPM;

    Vrl = (double)adc_rd * Vadc_33;            // For 3.3V Vcc use Vadc_33
    Rs = Rl * (3.3 - Vrl)/Vrl;                 // Calculate sensor resistance
    ratio = Rs/Rl;                             // Calculate ratio
    lgPPM = (log10(ratio) * -0.8) + 0.9;       // Calculate ppm
    ppm = 6* pow(10,lgPPM);                       // Calculate ppm
}

void CO2(){
    adc_rd = Co2.read_u16();
    wait_us(10000);
    CalculatePPM();
    printf("Environmental Sensor Value %d\n\r", ppm);
    wait_us(1000000);  
}



void LDR(){

}

int main(){
    // Set up the serial port to Baud 115200
    serial_port.set_baud(115200);

    Motor.motorSetup();
    Queue_Motor.call_every(1s, Motors);
    Queue_DHT11.call_every(1s, DHT11);
    Queue_CO2.call_every(1s, CO2);
    Queue_LDR.call_every(1s, LDR);

    //Thread_Motor.start(callback(&Queue_Motor, &EventQueue::dispatch_forever));
    Thread_DHT11.start(callback(&Queue_DHT11, &EventQueue::dispatch_forever));
    Thread_CO2.start(callback(&Queue_CO2, &EventQueue::dispatch_forever));
    //Thread_LDR.start(callback(&Queue_LDR, &EventQueue::dispatch_forever));
}