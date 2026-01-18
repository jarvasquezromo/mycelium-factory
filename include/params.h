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

#include <Preferences.h>

class ParamManager {
private:
    Preferences prefs;
public:
    float tempThreshold;
    float upperThHumidifier;
    float lowerThHumidifier;

    void begin() {
        prefs.begin("settings", false); // "settings" is the namespace
        // Load value, use 25.0 as default if not found
        tempThreshold = prefs.getFloat("threshold", 25.0); 
        upperThHumidifier = prefs.getFloat("upperThHumidifier", 80.0); 
        lowerThHumidifier = prefs.getFloat("lowerThHumidifier", 90.0); 
    }

    void saveThreshold(float val) {
        tempThreshold = val;
        prefs.putFloat("threshold", val);
    }

    void saveHumidifierThresholds () {
        prefs.putFloat ("upperThHumidifier", upperThHumidifier);
        prefs.putFloat ("lowerThHumidifier", lowerThHumidifier);
    }
};