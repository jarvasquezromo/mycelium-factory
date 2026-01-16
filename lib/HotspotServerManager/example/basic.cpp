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
#include "HotspotServerManager.h"

HotspotServerManager manager;
unsigned long lastUpdate = 0;

void handleWebCommand(String type, String msg) {
    Serial.println("Command received: " + msg);
    if(msg == "toggle_relay") {
        // Code to manually override relay
    }
}

void setup() {
    Serial.begin(115200);
    // Initialize: SSID, Password, Path to HTML
    manager.begin("Mycelium-Lab", "spores123", "/index.html");
    
    // Attach the callback function
    manager.onCommand(handleWebCommand);
}

void loop() {
    // Send data every 2 seconds
    if (millis() - lastUpdate > 2000) {
        // Create a JSON string to send to the browser
        String json = "{\"temp\": \"24.5\", \"hum\": \"88\"}"; 
        manager.broadcastData(json);
        lastUpdate = millis();
    }
}