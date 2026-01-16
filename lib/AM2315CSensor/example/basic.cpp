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

#include <Arduino.h>
#include "AM2315CSensor.h"

AM2315CSensor sensor(0x44);

void setup() {
    Serial.begin(115200);
    if (!sensor.begin(21, 22)) {
        Serial.println("Sensor not found!");
    }
}

void loop() {
    if (sensor.readSample() == ESP_OK) {
        Serial.printf("Temp: %.2f°C, Hum: %.2f%%\n", 
                      sensor.getTemperature(), 
                      sensor.getHumidity());
    }
    delay(2000);
}