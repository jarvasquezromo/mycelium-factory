
---

# HotspotServerManager Library

This library handles the creation of a **WiFi Access Point (Hotspot)** and hosts an **Asynchronous Web Server** with **WebSocket** support. It is designed for real-time monitoring of environmental data, such as humidity and temperature in mycelium cultivation.

## 🚀 Features

* **Zero-Lag:** Uses `ESPAsyncWebServer` so the ESP32 doesn't stop running while users browse the page.
* **Bi-directional:** Send sensor data to the browser and receive control commands (relays/settings) via WebSockets.
* **Separated Frontend:** Loads HTML directly from the `LittleFS` flash memory (keep your C++ clean).
* **JSON Ready:** Optimized for `ArduinoJson` to handle complex data packets.

---

## 📂 Installation

1. Ensure your project has the `data/` folder in the root directory.
2. In `platformio.ini`, add the following dependencies:
```ini
lib_deps =
    bblanchon/ArduinoJson @ ^6.21.3
    https://github.com/me-no-dev/ESPAsyncWebServer.git
    https://github.com/me-no-dev/AsyncTCP.git

```



---

## 🛠️ API Reference

### `begin(ssid, password, htmlPath)`

Initializes the WiFi Hotspot and mounts the filesystem.

* **ssid**: The name of the WiFi network.
* **password**: Minimum 8 characters.
* **htmlPath**: The path to your file in the data folder (e.g., `"/index.html"`).

### `broadcastData(jsonPayload)`

Sends a string (usually JSON) to all connected clients.

```cpp
manager.broadcastData("{\"hum\": 90}");

```

### `onCommand(callback)`

Registers a function to handle incoming messages from the web dashboard.

```cpp
manager.onCommand([](String type, String msg) {
    if(msg == "RELAY_ON") digitalWrite(PIN, HIGH);
});

```

---

## 💻 Communication Flow

1. **Client** connects to the ESP32 Hotspot.
2. **Browser** requests `192.168.4.1`.
3. **ESP32** serves the `index.html` from `LittleFS`.
4. **WebSocket** link is established.
5. **ESP32** pushes sensor updates every X seconds.
6. **Client** clicks a button, sending a string back to the `onCommand` listener.

---

## ⚠️ Requirements for Deployment

To use this library, you **must** upload the filesystem image to your ESP32:

1. Open the PlatformIO sidebar.
2. Under **Project Tasks**, find **Platform**.
3. Run **Build Filesystem Image**.
4. Run **Upload Filesystem Image**.

---

## 📜 License

Licensed under the **GNU General Public License v3.0**.

---