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
#include <ArduinoJson.h>

#include "config.h"
#include "params.h"
#include "AM2315CSensor.h"
#include "HotspotServerManager.h"
#include "HysteresisController.h"

/* Classes Initialization */
// Params
ParamManager p;
// Tempeture and Humidity sensor
AM2315CSensor sensor(Config::HUM_ADDRESS);
// Wifi manager
HotspotServerManager wifi_manager;
// Humidifier controller
HysteresisController humidifier(Config::PIN_HUM);

/* Variable Definitions */ 
// Variables of the program
unsigned long lastUpdate = 0;

/* Functions Declarations */
bool handleSensorData(JsonDocument & data);
void handleWebCommand(String type, String msg);

void setup() {
    // initiate parameter
    p.begin();

    // Initialize humidifier
    humidifier.begin();
    humidifier.setThreshols (p.lowerThHumidifier, p.upperThHumidifier);

    Serial.begin(Config::BAUDRATE_SERIAL);
    // Initialize: SSID, Password, Path to HTML
    wifi_manager.begin(Config::DEFAULT_SSID, Config::DEFAULT_PASS, "/index.html");
    
    // Attach the callback function
    wifi_manager.onCommand(handleWebCommand);

    if (!sensor.begin(Config::PIN_SDA, Config::PIN_SCL)) {
        Serial.println("Sensor not found!");
    }
}

void loop() {
    // Send data every interval
    if (millis() - lastUpdate > Config::SENSOR_INTERVAL) {
        
        // Create a JSON
        JsonDocument json_data;

        // Get values of the sensor
        handleSensorData(json_data);

        // Update controller
        humidifier.update (json_data["hum"].toFloat());

        // Write data
        wifi_manager.broadcastData(json_data);
        lastUpdate = millis();
    }
}

/* Functions definitions */
void handleWebCommand(String type, String msg) {
    Serial.println("Command received: " + msg);
    if(msg == "toggle_relay") {
        // Code to manually override relay
    }
}

bool handleSensorData(JsonDocument & data) {
    if (sensor.readSample() == ESP_OK) {
        data["temp"] = String(sensor.getTemperature(), 1);
        data["hum"] = String(sensor.getHumidity(), 1);
        data["status"] = "ok";

        return true;
    } else {
        data["status"] = "error";
        Serial.println("Error on sensor");
        return false;
    }
}