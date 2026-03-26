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

#ifndef DASHBOARD_MANAGER_H
#define DASHBOARD_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include <ESPDash.h>

#include "types.h"

class MyceliumDashboard {
private:
    AsyncWebServer  server;
    ESPDash         dashboard;

    dash::TemperatureCard<float, 2>     tempCard;
    dash::HumidityCard<float, 2>        humCard;
    dash::FeedbackCard<dash::string>    fanStatus;
    dash::ToggleButtonCard              heaterToggle;

public:
    MyceliumDashboard() : 
        server(80), 
        dashboard(server),
        tempCard(dashboard, "Inside Temperature"),
        humCard(dashboard, "Air Humidity"),
        fanStatus(dashboard, "System Status", dash::Status::NONE, "success"),
        heaterToggle(dashboard, "Manual Heater") 
    {}

    void begin() {
        heaterToggle.onChange([this](int value) {
            this->onHeaterChange(value);
        });
        server.begin();
    }

    void refresh(MyceliumData& data) {
        tempCard.setValue(data.temp);
        humCard.setValue(data.hum);
        
        fanStatus.setFeedback("success", dash::Status::INFO);
        dashboard.sendUpdates();
    }

    void onHeaterChange(int value) {
        heaterToggle.setValue(value);
        dashboard.sendUpdates();
    }
};

#endif