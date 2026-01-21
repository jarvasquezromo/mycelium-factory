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

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

namespace Config {
    // Hardware Pins
    constexpr int PIN_SDA = 21;
    constexpr int PIN_SCL = 22;
    constexpr int PIN_HUM =  2;
    
    // Default WiFi Settings
    const char* const DEFAULT_SSID = "Mycelium-Lab";
    const char* const DEFAULT_PASS = "spores123";
    
    // Timing
    constexpr unsigned long BAUDRATE_SERIAL = 115200;
    constexpr unsigned long SENSOR_INTERVAL = 2000;

    // I2C communication
    constexpr int HUM_ADDRESS = 0x44;
}

#endif