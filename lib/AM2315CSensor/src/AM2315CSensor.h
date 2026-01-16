/*
 * Copyright (C) 2026  Joaquin Vasquez Romo
 * * This program is free software: you can redistribute it and/or modify
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SHT_SENSOR_H
#define SHT_SENSOR_H

#include <Arduino.h>
#include <Wire.h>

class AM2315CSensor  {
public:
    /**
     * @brief Constructor
     * @param address I2C address (usually 0x44 or 0x45)
     */
    AM2315CSensor (uint8_t address = 0x44);

    /**
     * @brief Initializes the I2C bus and verifies sensor presence
     * @return true if successfully initialized
     */
    bool begin(int sda, int scl, uint32_t frequency = 100000);

    /**
     * @brief Triggers a measurement and reads the data
     * @return esp_err_t ESP_OK if successful
     */
    esp_err_t readSample();

    float getTemperature() const { return _temperature; }
    float getHumidity() const { return _humidity; }

private:
    uint8_t _address;
    float _temperature;
    float _humidity;

    uint8_t crc8(const uint8_t *data, int len);
};

#endif