#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "SalesData.h"  // Include SalesData class to manage sales data
#include "config.h"     // Include configuration file for credentials and settings
#include "images/creem.h"

// Initialize the display
TFT_eSPI tft = TFT_eSPI();
#define TFT_BL 4
unsigned long lastUpdate = 0;
// Define screen dimensions
#define TFT_WIDTH 320
#define TFT_HEIGHT 240

// Global variables for tracking
SalesData salesData;  // Create an instance of SalesData class

void setup() {
    Serial.begin(115200);
    SPI.begin();
    SPI.setFrequency(27000000); // Try a lower frequency like 27MHz
    
    // Initialize display
    tft.init();
    tft.setRotation(3);  // Ensure the rotation is set correctly
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);

    // Initial display setup
    initializeDisplay();

    // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    updateStatus("Connecting to WiFi...");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("...");
    }
    
    updateStatus("WiFi Connected");
}

void drawImageWithTransparency(int x, float y, int width, int height, const unsigned short* image, unsigned short transparentColor) { // Remove black background for transparency
    for (int i = 0; i < width * height; i++) {
        unsigned short pixel = pgm_read_word(&(image[i]));  // Reading the image from flash memory
        if (pixel != transparentColor) {
            int px = x + (i % width);
            int py = y + (i / width);
            tft.drawPixel(px, py, pixel);
        }
    }
}

void initializeDisplay() {
    // Clear the screen
    tft.fillScreen(BACKGROUND_COLOR);
    delay(10);

    // Header with logo and title
    tft.fillRect(0, 0, TFT_WIDTH, TFT_HEIGHT * 0.13, HEADER_COLOR); // Slightly increased header height
    tft.setTextColor(TITLE_COLOR);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawString(TITLE, TFT_WIDTH / 2, TFT_HEIGHT * 0.06);

    // Draw logo in the header
    tft.setSwapBytes(true);
    drawImageWithTransparency(10, 2.5, 25, 25, creem, 0x0000);

    // Draw smaller cards for total sales count and latest sale
    salesData.drawCard(10, TFT_HEIGHT * 0.17, TFT_WIDTH / 2 - 25, TFT_HEIGHT * 0.2, CARD_COLOR, "Total Sales", "0");
    salesData.drawCard(TFT_WIDTH / 2 + 5, TFT_HEIGHT * 0.17, TFT_WIDTH / 2 - 25, TFT_HEIGHT * 0.2, CARD_COLOR, "Latest Sale", "$0.00");

    // Draw card for total sales in dollars at the bottom
    tft.drawFastHLine(0, TFT_HEIGHT * 0.75, TFT_WIDTH, HEADER_COLOR);  // Divider line
}

void updateStatus(const char* status) {
    tft.fillRect(0, TFT_HEIGHT * 0.75, TFT_WIDTH, TFT_HEIGHT * 0.1, BACKGROUND_COLOR);
    tft.setTextColor(TEXT_COLOR);
    tft.setTextSize(1);
    tft.setCursor(5, TFT_HEIGHT * 0.78);
    tft.print(status);
}

void loop() {
    unsigned long currentMillis = millis();
    
    if (currentMillis - lastUpdate >= 60000 || lastUpdate == 0) {
        if (WiFi.status() == WL_CONNECTED) {
            salesData.updateSalesData(SERVER_URL)  // Fetch and update the sales data every minute
            lastUpdate = currentMillis;
        } else {
            updateStatus("WiFi Disconnected");
        }
    }
}
