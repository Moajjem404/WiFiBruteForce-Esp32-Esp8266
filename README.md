# 🌐 ESP WiFi Brute Force Tool

This repository contains a **WiFi Scanner and Brute Force Tool** built for both ESP32 and ESP8266 microcontrollers. It allows scanning of nearby WiFi networks and brute-forcing WiFi passwords using a custom wordlist.

## ✨ Features

- 🔍 Scan for nearby WiFi networks.
- 📶 Display network signal strength (RSSI).
- 📂 Upload a custom wordlist file for brute-forcing.
- ⏱️ Real-time feedback during the brute-force process.
- ⚙️ Separate implementations for **ESP32** and **ESP8266**.

## 📁 Folder Structure

```
EWiFiBruteForce-Esp32-Esp8266/
├── Esp32.ino                 # Code for ESP32
├── Esp8266.ino               # Code for ESP8266
├── wordlist.txt              # Sample wordlist file
├── README.md                 # Documentation
└── LICENSE                   # License file
```

## 🛠️ Requirements

### 🖥️ Hardware
- **ESP32** or **ESP8266** microcontroller.
- USB cable for programming.

### 🧰 Software
- [Arduino IDE](https://www.arduino.cc/en/software) or any other compatible editor.
- ESP32 or ESP8266 board manager installed in the Arduino IDE.

## 🚀 Installation and Setup

### 1️⃣ Clone the Repository
```bash
git clone https://github.com/your-username/ESP-WiFi-Scanner-and-BruteForce.git
cd WiFiBruteForce-Esp32-Esp8266
```

### 2️⃣ Open the Code
- For ESP32: Open `Esp32.ino` in Arduino IDE.
- For ESP8266: Open `Esp8266.ino` in Arduino IDE.

### 3️⃣ Install Required Libraries
Make sure the following libraries are installed in the Arduino IDE:
- **WiFi.h** (for ESP32)
- **ESP8266WiFi.h** (for ESP8266)
- **WebServer.h**
- **SPIFFS.h**

### 4️⃣ Configure WiFi Access Point
In the `.ino` file, modify the following lines if needed:
```cpp
const char* apSSID = "ShadowGlint";      // Default Access Point SSID
const char* apPassword = "12345678";    // Default Password
```

### 5️⃣ Upload the Code
1. Connect your ESP32 or ESP8266 to your computer.
2. Select the appropriate board and COM port in the Arduino IDE.
3. Click **Upload** to flash the code to the microcontroller.

### 6️⃣ Upload Wordlist (Optional)
- Use the web interface to upload a custom wordlist file (one password per line).
- A sample wordlist file (`wordlist.txt`) is included for reference.

## 📝 How to Use

1. 🔗 Connect to the WiFi network broadcasted by the ESP device.
   - Default SSID: `ShadowGlint`
   - Default Password: `12345678`

2. 🌐 Open a web browser and navigate to:
   ```
   http://192.168.4.1
   ```

3. From the web interface:
   - 🔍 Scan for nearby WiFi networks.
   - 🎯 Select a target network.
   - 📂 Upload a wordlist file for brute-forcing.
   - ⏱️ Start the brute-force process and monitor progress in real time.

## ⚠️ Notes
- **🚨 Use Responsibly:** This tool is intended for educational purposes only. Unauthorized access to networks is illegal and unethical.
- **📏 Wordlist Size:** Ensure the wordlist file is not excessively large (preferably < 5MB).


## 🤝 Contributing
Contributions are welcome! Feel free to submit pull requests or report issues.

## 👤 Author
Created by **@ShadowGlint**.

