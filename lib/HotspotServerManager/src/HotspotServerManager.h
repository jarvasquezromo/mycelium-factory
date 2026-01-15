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

#ifndef HOTSPOT_SERVER_MANAGER_H
#define HOTSPOT_SERVER_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h> // Required for clean data exchange

class HotspotServerManager {
public:
    // Function signature for the command callback
    typedef std::function<void(String, String)> CommandCallback;

    HotspotServerManager();
    void begin(const char* ssid, const char* password, const char* htmlPath);
    
    // Send data to the web page (e.g., {"temp": 25.5, "hum": 90})
    void broadcastData(String jsonPayload);
    
    // Register a function to handle clicks from the web page
    void onCommand(CommandCallback cb);

private:
    AsyncWebServer _server;
    AsyncWebSocket _ws;
    const char* _htmlPath;
    CommandCallback _onCommand;

    void _onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, 
                  AwsEventType type, void *arg, uint8_t *data, size_t len);
};

#endif