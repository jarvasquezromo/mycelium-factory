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

 #include "HotspotServerManager.h"

HotspotServerManager::HotspotServerManager() : _server(80), _ws("/ws") {}

void HotspotServerManager::begin(const char* ssid, const char* password, const char* htmlPath) {
    _htmlPath = htmlPath;
    
    if(!LittleFS.begin(true)) return;

    WiFi.softAP(ssid, password);

    // Attach WebSocket handler
    _ws.onEvent(std::bind(&HotspotServerManager::_onEvent, this, 
                std::placeholders::_1, std::placeholders::_2, 
                std::placeholders::_3, std::placeholders::_4, 
                std::placeholders::_5, std::placeholders::_6));
    _server.addHandler(&_ws);

    // Serve Static HTML
    _server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
        request->send(LittleFS, _htmlPath, "text/html");
    });

    _server.begin();
}

void HotspotServerManager::broadcastData(String jsonPayload) {
    _ws.textAll(jsonPayload);
}

void HotspotServerManager::onCommand(CommandCallback cb) {
    _onCommand = cb;
}

void HotspotServerManager::_onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, 
                                    AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_DATA) {
        // Handle incoming messages from the browser
        String msg = "";
        for(size_t i=0; i<len; i++) msg += (char)data[i];
        
        if(_onCommand) _onCommand("web_input", msg); 
    }
}