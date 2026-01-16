#include <Arduino.h>

#include "AM2315CSensor.h"
#include "HotspotServerManager.h"

/* Classes Initialization */
// Tempeture and Humidity sensor
AM2315CSensor sensor(0x44);
// Wifi manager
HotspotServerManager wifi_manager;

/* Variable Definitions */ 
// Variables of the program
unsigned long lastUpdate = 0;

/* Functions Declarations */
bool handleSensorData(String json_type);
void handleWebCommand(String type, String msg);

void setup() {
    Serial.begin(115200);
    // Initialize: SSID, Password, Path to HTML
    wifi_manager.begin("Mycelium-Lab", "spores123", "/index.html");
    
    // Attach the callback function
    wifi_manager.onCommand(handleWebCommand);

    if (!sensor.begin(21, 22)) {
        Serial.println("Sensor not found!");
    }
}

void loop() {
    // Send data every 2 seconds
    if (millis() - lastUpdate > 2000) {
        
        // Create a JSON
        String json;

        // Get values of the sensor
        handleSensorData(json);

        // Write data
        wifi_manager.broadcastData(json);
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

bool handleSensorData(String json_type) {
    if (sensor.readSample() == ESP_OK) {
        json_type += "\"temp\":\"" + String(sensor.getTemperature(), 1) + "\",";
        json_type += "\"hum\":\"" + String(sensor.getHumidity(), 1) + "\",";
        json_type += "\"status\":\"ok\",";
        
        // Output: {"temp":24.5,"hum":88.2,"status":"ok"}
        Serial.println(json_type); 
        return true;
    } else {
        json_type += "\"status\":\"error\",";
        Serial.println("Error on sensor");
        return false;
    }
}