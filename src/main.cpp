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

#include "types.h"
#include "config.h"
#include "params.h"
#include "DashboardManager.h"

#include "AM2315CSensor.h"
#include "HotspotServerManager.h"
#include "HysteresisController.h"

/* Classes Initialization */
// Params
ParamManager p;
// Tempeture and Humidity sensor
AM2315CSensor sensor(Config::HUM_ADDRESS);
// Humidifier controller
HysteresisController humidifier(Config::PIN_HUM);
// Dashboard
MyceliumDashboard dashboard;

/* Variable Definitions */ 
// Variables of the program
MyceliumData data;
unsigned long lastUpdate = 0;


/* Functions Declarations */
bool handleSensorData(MyceliumData & data);
void handleHumidifierData(MyceliumData & data);
// void handleWebCommand(String type, String msg);

void setup() {
    // initiate parameter
    p.begin();

    // Initialize humidifier
    humidifier.begin();
    humidifier.setThresholds (p.lowerThHumidifier, p.upperThHumidifier);

    // Communication
    Serial.begin(Config::BAUDRATE_SERIAL);
    // Initialize wifi: SSID, Password
    WiFi.begin(Config::DEFAULT_SSID, Config::DEFAULT_PASS);
    while (WiFi.status() != WL_CONNECTED) { delay(500); }

    if (!sensor.begin(Config::PIN_SDA, Config::PIN_SCL)) {
        Serial.println("Sensor not found!");
    }

    // Start dashboard
    dashboard.begin ();
}

void loop() {
    // Send data every interval
    if (millis() - lastUpdate > Config::SENSOR_INTERVAL) {

        // Get values of the sensor
        handleSensorData(data);

        // Update controller
        humidifier.update (data.hum);
        handleHumidifierData (data);

        // Update dashboard
        dashboard.refresh (data);
        lastUpdate = millis();
    }
}

/* Functions definitions */
// void handleWebCommand(String type, String msg) {
//     Serial.println("Command received: " + msg);
//     if(msg == "toggle_relay") {
//         // Code to manually override relay
//         humidifier.setMode (ControllerMode::MANUAL);
//         humidifier.tonggleManualPower ();
//     }
// }

bool handleSensorData(MyceliumData & data) {
    if (sensor.readSample() == ESP_OK) {
        data.temp = sensor.getTemperature();
        data.hum = sensor.getHumidity();
        data.status = "ok";

        return true;
    } else {
        data.status = "error";
        Serial.println("Error on sensor");
        return false;
    }
}

void handleHumidifierData(MyceliumData & data) {
    data.hum_state = humidifier.isActive();
}
