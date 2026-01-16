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

#include "AM2315CSensor.h"

// The AM2315C has a fixed address of 0x38
AM2315CSensor::AM2315CSensor(uint8_t address) : _address(0x38), _temperature(0.0f), _humidity(0.0f) {}

bool AM2315CSensor::begin(int sda, int scl, uint32_t frequency) {
    bool ok = Wire.begin(sda, scl, frequency);
    delay(100); // Power on delay per datasheet
    return ok;
}

esp_err_t AM2315CSensor::readSample() {
    // 1. Trigger Measurement Command (0xAC, 0x33, 0x00)
    Wire.beginTransmission(_address);
    Wire.write(0xAC);
    Wire.write(0x33);
    Wire.write(0x00);
    if (Wire.endTransmission() != 0) return ESP_FAIL;

    // 2. Wait for measurement to complete (Datasheet specifies >80ms)
    delay(85); 

    // 3. Request 7 bytes: [Status, Hum1, Hum2, Hum3/Temp1, Temp2, Temp3, CRC]
    Wire.requestFrom(_address, (uint8_t)7);
    if (Wire.available() != 7) return ESP_ERR_TIMEOUT;

    uint8_t data[7];
    for (int i = 0; i < 7; i++) {
        data[i] = Wire.read();
    }

    // 4. Check Status Bit (Bit 7 of first byte should be 0, meaning idle/ready)
    if ((data[0] & 0x80) != 0) return ESP_ERR_INVALID_STATE;

    // 5. Convert 20-bit Raw Data
    // Humidity: Data[1], Data[2], and high 4 bits of Data[3]
    uint32_t rawHum = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (data[3] >> 4);
    
    // Temperature: Low 4 bits of Data[3], Data[4], and Data[5]
    uint32_t rawTemp = ((uint32_t)(data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];

    // 6. Final conversion formulas from Datasheet
    // RH = (Srh / 2^20) * 100%
    _humidity = ((float)rawHum / 1048576.0f) * 100.0f;
    
    // T = (St / 2^20) * 200 - 50
    _temperature = ((float)rawTemp / 1048576.0f) * 200.0f - 50.0f;

    return ESP_OK;
}

// AM2315C uses a standard CRC-8 if needed, but usually 
// verification of the status byte is sufficient for most applications.
uint8_t AM2315CSensor::crc8(const uint8_t *data, int len) {
    uint8_t crc = 0xFF;
    for (int j = len; j > 0; --j) {
        crc ^= *data++;
        for (int i = 8; i > 0; --i) {
            crc = (crc & 0x80) ? (crc << 1) ^ 0x31 : (crc << 1);
        }
    }
    return crc;
}