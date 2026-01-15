
---

# Mycelium Factory: Environmental Controller

A asynchronous ESP32-based environmental controller designed for mycelium cultivation. This project manages humidity via a relay using a hysteresis (bang-bang) control logic and provides a real-time web dashboard via a local WiFi Hotspot.

## 🚀 Features

* **Asynchronous Web Server**: The UI does not block the sensor reading or relay logic.
* **Live Dashboard**: Uses WebSockets to push Temperature and Humidity data to your phone instantly.
* **WiFi Hotspot**: No internet required; the ESP32 creates its own network.
* **Hysteresis Control**: Prevents relay "chattering" by using upper and lower humidity thresholds.
* **Separated Frontend**: HTML/CSS is stored in `LittleFS` filesystem, keeping the C++ logic clean.

---

## 🛠️ Hardware Requirements

* **Microcontroller**: ESP32 (Feather or DevKit).
* **Sensor**: DHT22 or SHT3x (Humidity & Temperature).
* **Actuator**: 5V Relay Module (Optoisolated recommended).
* **Power**: AC Humidifier.

---

## 📂 Project Structure

```text
.
├── data/                  # Web Dashboard (HTML/CSS)
├── lib/                   
│   └── HotspotServerManager/ # Custom Library for WiFi/Web
├── src/                   
│   └── main.cpp           # Main Control Logic
└── platformio.ini         # Environment & Dependency config

```

---

## ⚙️ Setup & Installation

### 1. Library Dependencies

Ensure your `platformio.ini` includes:

```ini
lib_deps =
    bblanchon/ArduinoJson @ ^6.21.3
    https://github.com/me-no-dev/ESPAsyncWebServer.git
    https://github.com/me-no-dev/AsyncTCP.git
    adafruit/DHT sensor library

```

### 2. The Special Step: Uploading the Web Page (HTML)

Since the HTML is separated from the code, you must upload the **Filesystem Image** to the ESP32 memory. Simply clicking the "Upload" arrow is **not enough**.

1. Close the **Serial Monitor** in VS Code.
2. Click the **PlatformIO (Alien) Icon** on the left sidebar.
3. Navigate to: `Project Tasks` > `featheresp32` > **`Platform`**.
4. Run **`Build Filesystem Image`**.
5. Run **`Upload Filesystem Image`**.

---

## 📱 How to Use

1. Connect your phone to the WiFi network: **Mycelium-Lab**.
2. Open your browser and navigate to: `http://192.168.4.1`.
3. The dashboard will automatically update with live sensor data.

---

## ⚠️ Safety Disclaimer

**WARNING: High Voltage AC Electricity.**
This project involves interfacing with AC mains power.

* Improper wiring can result in **electric shock, fire, or death**.
* Always disconnect AC power before touching wiring.
* The authors are not responsible for any damage or injury. Use at your own risk.

---

## 📜 License

This project is licensed under the **GNU General Public License v3.0**.
See the [LICENSE](https://www.google.com/search?q=LICENSE) file for the full text.
Copyright (C) 2026 [Your Name/Group Name].

---