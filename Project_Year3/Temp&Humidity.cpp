#include "Temp&Humidity.h"

Dht11::Dht11(PinName const &p) : DHT11pin(p){
    DHT11Startup.start();
    temperature = 0;
    humidity = 0;
    //Startup requires at leats 1 second before readings
}

int Dht11::readDHT11(){
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

    // ACKNOWLEDGE or TIMEOUT
    unsigned int loopCnt = 10000;
    while(DHT11pin == 0)
        if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

    loopCnt = 10000;
    while(DHT11pin == 1)
        if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

    // READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
    for (int i=0; i<40; i++)
    {
        loopCnt = 10000;
        while(DHT11pin == 0)
            if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

        //unsigned long t = micros();
        Timer t;
        t. start();

        loopCnt = 10000;
        while(DHT11pin == 1)
            if (loopCnt-- == 0) return DHTLIB_ERROR_TIMEOUT;

        if (t.read_us() > 40) bits[idx] |= (1 << cnt);
        if (cnt == 0)   // next byte?
        {
            cnt = 7;    // restart at MSB
            idx++;      // next byte!
        }
        else cnt--;
    }


    //Write humidity and temperature
    humidity = bits[0];
    temperature = bits[2];

    uint8_t sum = bits[0] + bits[2];  

    if (bits[4] != sum) return DHTLIB_ERROR_CHECKSUM;
    return DHTLIB_OK;
}

float Dht11::getFahrenheit() {
    return((temperature * 1.8) + 32);
}

int Dht11::getCelsius() {
    return(temperature);
}

int Dht11::getHumidity() {
    return(humidity);
}

