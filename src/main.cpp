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