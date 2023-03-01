
#include "Temp&Humidity.h"
Dht11::Dht11(PinName const &p) : DHT11pin(p){
    DHT11Startup.start();
    temperature = 0;
    humidity = 0;
    //Startup requires at leats 1 second before readings
}

int Dht11::read(){
    //Data receive buffer
    uint8_t bits[5];
    uint8_t cnt = 7;
    uint8_t idx = 0;

    //Empty the buffer
    for (int i=0; i<5; i++) bits[i] = 0;

     while(DHT11Startup.read_ms() < 1100) {}
    DHT11Startup.stop();

    //Tell the sensor we are ready to read, and set the pin to input
    DHT11pin.output();
    DHT11pin = 0;
    wait_us(18000);
    DHT11pin = 1;
    wait_us(40);
    DHT11pin.input();
}