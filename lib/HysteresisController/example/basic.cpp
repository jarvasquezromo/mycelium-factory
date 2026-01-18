#include <Arduino.h>
#include <ArduinoJson.h>
#include "HysteresisController.h"
#include "AM2315CSensor.h"

// Initialize on Pin 13, Range: 80% to 90% Humidity
HysteresisController humidifier(13, 80.0, 90.0);
AM2315CSensor sensor(0x44);

void setup() {
    Serial.begin(115200);
    humidifier.begin();
    sensor.begin(21, 22);
}

void loop() {
    static unsigned long lastTick = 0;

    // Check sensor and update logic
    if (sensor.readSample() == ESP_OK) {
        float currentHum = sensor.getHumidity();
        
        // 1. Run the controller logic
        humidifier.update(currentHum);

        // 2. Every 2 seconds, report status via JSON
        if (millis() - lastTick > 2000) {
            JsonDocument doc;
            doc["humidity"] = serialized(String(currentHum, 1));
            doc["relay_active"] = humidifier.isActive();
            doc["mode"] = (humidifier.getMode() == ControllerMode::AUTOMATIC) ? "AUTO" : "MANUAL";
            
            // Add settings to the JSON so the Web App knows the thresholds
            doc["config"]["low"] = humidifier.getLowThreshold();
            doc["config"]["high"] = humidifier.getHighThreshold();

            String output;
            serializeJson(doc, output);
            Serial.println(output);
            
            lastTick = millis();
        }
    }
}

// Example of how your Web Command would switch modes
void handleWebCommand(String type, String msg) {
    if (type == "MODE_CHANGE") {
        if (msg == "AUTO") humidifier.setMode(ControllerMode::AUTOMATIC);
        else humidifier.setMode(ControllerMode::MANUAL);
    }
    
    if (type == "MANUAL_TOGGLE") {
        humidifier.setManualPower(msg == "ON");
    }
}