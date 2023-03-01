#include "mbed.h"
#include "Temp&Humidity.h"
#include <iostream>
#include <chrono>

using namespace std;

Dht11 DHT11pin(PF_15);

Thread Thread_Motor;
Thread Thread_DHT11;

EventQueue Queue_Motor(1 * EVENTS_EVENT_SIZE);
EventQueue Queue_DHT11(1 * EVENTS_EVENT_SIZE);

void Motors(){
    void LineFollowing();
}

void DHT11(){
    DHT11pin.read();
    int TEMP = DHT11pin.getCelsius();
    //int TEMP = DHT11.getFahrenheit();
    printf("T: %d, H: %d\r\n", TEMP, DHT11pin.getHumidity());
}



int main(){
    void motorSetup();
    Queue_Motor.call_every(1ms, Motors);
    Queue_DHT11.call_every(1s, DHT11);

    Thread_Motor.start(callback(&Queue_Motor, &EventQueue::dispatch_forever));
    Thread_DHT11.start(callback(&Queue_DHT11, &EventQueue::dispatch_forever));
}