#ifndef MILLCONTROL_SCALE_H
#define MILLCONTROL_SCALE_H

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

class Scale {
public:
    static const unsigned char BUFFER_SIZE = 32;

private:
    static const unsigned char EEPROM_VERSION = 4;

    //The hx711 pinout
    const unsigned char data_pin;
    const unsigned char clock_pin;

    //needed for power up & down
    const unsigned char power_pin;

    //Tare offest
    long offset;
    //Noise level
    int noise = 0;
    //Calibration factor
    double factor = -1;

    //Weight buffer - gives a floating average of measurements
    long weight_buffer[BUFFER_SIZE];
    long buffer_sum;
    unsigned char buffer_pos;

    void eeprom_read();

    void eeprom_write();

public:
    Scale(unsigned char data_pin, unsigned char clock_pin, unsigned char power_pin);

    void power_down();

    void power_up();

    long read();

    long read(unsigned char times);

    long stable_read(unsigned char times = 20);

    void tare(unsigned char times = 20);

    long get_weight(unsigned char times = 10);

    long get_stable_weight(unsigned char times = 20);

    //calibration
    void set_weight(long weight, unsigned char times = 100);

    //noise calibration
    void calibrate_noise(unsigned char times = 100);

    void reset_floating_weight();

    //returns the floating average - must be initialized first
    long get_floating_weight(unsigned char updates = 1);

    long toWeight(long value);
};

#endif //MILLCONTROL_SCALE_H
