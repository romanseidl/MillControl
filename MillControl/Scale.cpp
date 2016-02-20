/**
 * HX 711 based scale
 * (c) 2016 by Roman Seidl
 * code partly based on code by @mamu at kaffee-netz.de and on the 
 * Q2HX711 library by Scott Russell
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Scale.h"

#include <avr/eeprom.h>
#include <Arduino.h>


EEMEM unsigned char eeprom_version;
EEMEM int eeprom_noise;
EEMEM long eeprom_offset;
EEMEM double eeprom_factor;

Scale::Scale(unsigned char data_pin, unsigned char clock_pin, unsigned char power_pin) : data_pin(data_pin),
                                                                                         clock_pin(clock_pin),
                                                                                         power_pin(power_pin) {
    pinMode(data_pin, INPUT);
    pinMode(clock_pin, OUTPUT);
    pinMode(power_pin, OUTPUT);
    digitalWrite(power_pin, HIGH);
    reset_floating_weight();
    eeprom_read();
};


long Scale::read() {
    while (digitalRead(data_pin));

    noInterrupts();

    //read data
    byte data[3];
    for (byte j = 3; j--;) {
        data[j] = shiftIn(data_pin, clock_pin, MSBFIRST);
    }

    //Will produce GAIN 128
    digitalWrite(clock_pin, HIGH);
    digitalWrite(clock_pin, LOW);

    interrupts();

    data[2] ^= 0x80;
    //merge to long
    return ((uint32_t) data[2] << 16) | ((uint32_t) data[1] << 8) | (uint32_t) data[0];
}

long lround(double value) {
    return round(value);
}

//Returns the average of n raw weight measurements
long Scale::read(unsigned char times) {
    long sum = 0;
    for (unsigned char i = 0; i < times; i++) {
        sum += read();
    }
    return lround((double) sum / (double) times);
}

long Scale::toWeight(long value) {
    return lround(value * factor);
}

//Returns an average weight of n meassurements - only works if calibrated els -1
long Scale::get_weight(unsigned char times) {
    return toWeight(read(times) - offset);
}

//Calibrates the scale with measurement
void Scale::set_weight(long weight, unsigned char times) {
    long data = read(times) - offset;
    factor = (double) weight / (double) (data);
    eeprom_write();
}

//Initializes the floating weight buffer
void Scale::reset_floating_weight() {
    buffer_pos = 0;
    buffer_sum = 0;
    for (unsigned char p = 0; p < BUFFER_SIZE; p++)
        weight_buffer[p] = 0;
}

//Reads a number of bytes, adds them to the buffer and returns the current average
long Scale::get_floating_weight(unsigned char updates) {
    for (unsigned char u = 0; u < updates; u++) {
        buffer_sum -= weight_buffer[buffer_pos];

        const long data = (read() - offset);
        weight_buffer[buffer_pos] = data;
        buffer_sum += data;

        buffer_pos = (buffer_pos + 1) % BUFFER_SIZE;
    }
    return toWeight(((double) buffer_sum / (double) BUFFER_SIZE));
}

long Scale::get_stable_weight(unsigned char times) {
    return toWeight(stable_read(times) - offset);
}

//meassures the noise level the scale has by running a series of measurements on the scale
void Scale::calibrate_noise(unsigned char times) {
    long value = read();
    for (unsigned char i = 0; i < times; i++) {
        long next_value = read();
        long delta = abs(next_value - value);
        if (noise < delta) {
            noise = delta;
        }
        value = next_value;
    }
    eeprom_write();
}

//reads a value if the scale is stable
long Scale::stable_read(unsigned char times) {
    if (!noise)
        calibrate_noise(100);

    long value;
    unsigned char counter = 0;
    long sum = 0;
    do {
        long next_value = read();
        if (abs(value - next_value) > noise) {
            //restart the process as there was instability
            sum = counter = 0;
        } else {
            //add result
            sum += next_value;
            counter++;
        }
        value = next_value;
    } while (counter < times);

    //calculate average
    return sum / times;
}


//tare the scale
void Scale::tare(unsigned char times) {
    offset = stable_read(times);
    reset_floating_weight();
}

//Power down the Sensor and the power pin
void Scale::power_down() {
    digitalWrite(power_pin, HIGH);
    digitalWrite(clock_pin, LOW);
    digitalWrite(clock_pin, HIGH);
}

//Power up the Sensor and the power pin
void Scale::power_up() {
    digitalWrite(power_pin, LOW);
    digitalWrite(clock_pin, LOW);
}

//Read calibration data from eeprom
void Scale::eeprom_read() {
    if (EEPROM_VERSION == eeprom_read_byte(&eeprom_version)) {
        eeprom_read_block(&noise, &eeprom_noise, sizeof(int));
        eeprom_read_block(&offset, &eeprom_offset, sizeof(long));
        eeprom_read_block(&factor, &eeprom_factor, sizeof(double));
    }
}

//Write calibration data to eeprom
void Scale::eeprom_write() {
    eeprom_write_byte(&eeprom_version, EEPROM_VERSION);
    eeprom_update_block(&noise, &eeprom_noise, sizeof(int));
    eeprom_update_block(&offset, &eeprom_offset, sizeof(long));
    eeprom_update_block(&factor, &eeprom_factor, sizeof(double));
}
