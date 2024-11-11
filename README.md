# Sales Dashboard

A simple dashboard to display sales data on an ESP32 using TFT screen.

## Features:
- Displays total sales, latest sale, and total sales amount
- Connects to a server to fetch sales data via HTTP request
- Displays sales data on a TFT screen

## Requirements:
- Arduino IDE
- ESP32 Board Package
- Libraries: WiFi, HTTPClient, ArduinoJson, TFT_eSPI

## Setup:
1. Install Arduino IDE:
Download and install the Arduino IDE.

2. Install ESP32 in Arduino IDE:
Open Arduino IDE > File > Preferences.
Add this URL to Additional Boards Manager 

https://dl.espressif.com/dl/package_esp32_index.json

Go to Tools > Board > Boards Manager and install ESP32 by Espressif.

3. Select ESP32 Board:
Go to Tools > Board and select your ESP32 board (e.g., ESP32 Dev Module).
Select the correct Port under Tools > Port.

4. Modify Configuration:
Open config.h and set:

#define WIFI_SSID     "your-SSID"

#define WIFI_PASSWORD "your-PASSWORD"

#define SERVER_URL "http://your-server-url.com/api/sales"

#define TITLE "XBlockBot Sales"


5. Upload Code:
Open creem_monitoring.ino in Arduino IDE.
Click Upload to compile and upload the code to ESP32.

## License:
MIT License
