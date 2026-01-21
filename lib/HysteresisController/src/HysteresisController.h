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

#ifndef HYSTERESIS_CONTROLLER_H
#define HYSTERESIS_CONTROLLER_H

#include <Arduino.h>

enum class ControllerMode { MANUAL, AUTOMATIC };

class HysteresisController {
public:
    HysteresisController(uint8_t pin);
    
    void begin();
    void update(float currentReading);
    
    void setMode(ControllerMode m);
    void setManualPower(bool on);
    void tonggleManualPower();
    void setThresholds(float low, float high);

    bool isActive() const;
    ControllerMode getMode() const;

private:
    uint8_t _pin;
    float _targetLow = 80.0;
    float _targetHigh = 90.0;
    bool _state;
    ControllerMode _mode;
    bool _manualState;
};

#endif