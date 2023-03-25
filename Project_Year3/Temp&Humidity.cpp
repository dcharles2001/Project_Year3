#include "Temp&Humidity.h"
DigitalInOut DHT11pin(D8);                  // Sensor pin

void Dht11::DHT11setup(){
    DHT11Startup.start();                   // Start the setup timer
    temperature = 0;                        // Set temp to 0
    humidity = 0;                           // set humidity to 0
}

int Dht11::readDHT11(){
    //Data receive buffer
    uint8_t bits[5];
    uint8_t count = 7;
    uint8_t idx = 0;

    //Empty the buffer
    for (int i=0; i<5; i++){
        bits[i] = 0;
    }

    while(DHT11Startup.read_ms() < 1100) {} // Blocking while loop because startup requires at leats 1 second before readings are avaliable
    DHT11Startup.stop();                    // Stop the setup timer

    //Tell the sensor we are ready to read
    DHT11pin.output();
    DHT11pin = 0;
    wait_us(18000);
    DHT11pin = 1;
    wait_us(40);
    DHT11pin.input();                       // Set the sensor to read


    unsigned int loop = 10000;

    while(DHT11pin == 0){                   // give an error if the pin is stuck at 0 for more than 10000 loops
        if (loop-- == 0){
            return DHT_ERROR_TIMEOUT;
        }
    }
    loop = 10000;

    while(DHT11pin == 1){                   // give an error if the pin is stuck at 1 for more than 10000 loops
        if (loop-- == 0){
            return DHT_ERROR_TIMEOUT;
        }
    }

    // READ OUTPUT - 40 BITS => 5 BYTES or TIMEOUT
    for (int i=0; i<40; i++)
    {
        loop = 10000;
        while(DHT11pin == 0){               // give an error if the pin is stuck at 0 for more than 10000 loops
            if (loop-- == 0){
                return DHT_ERROR_TIMEOUT;
            }
        }

        Timer bitread;                      // Start timer for reading bits
        bitread. start();

        loop = 10000;
        while(DHT11pin == 1){               // give an error if the pin is stuck at 1 for more than 10000 loops
            if (loop-- == 0){
                return DHT_ERROR_TIMEOUT;
            }
        }

        if (bitread.read_us() > 40){        // if the timer is more than 40 do the following
            bits[idx] |= (1 << count);      // Do an OR operation between bits[idx] and (1<<count) and assign the result to bits[idx]
        }        
        if (count == 0){                    // If the count is 0 restart count (7) and move to next byte
            count = 7;
            idx++;
        }else{
            count--;                        // Else remove one from count
        }                       
    }


    //Write humidity and temperature
    humidity = bits[0];
    temperature = bits[2];

    uint8_t sum = bits[0] + bits[2];        // Take the sum of bits 0 and 2

    if (bits[4] != sum){                    // If the sum of 0 and 2 are not equal to bits 4 return an error
        return DHT_ERROR_CHECKSUM;
    }
    return DHT_OK;                          // If all is good in the checksum, return an OK
}

int Dht11::getFahrenheit(){
    return((temperature * 1.8) + 32);       // Work out and return Fahrenheit
}

int Dht11::getCelsius(){
    return(temperature);                    // Return Celsius
}

int Dht11::getHumidity(){
    return(humidity);                       // Return Humidity
}

