#ifndef SALESDATA_H
#define SALESDATA_H

#include "config.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>
extern TFT_eSPI tft;

class SalesData {
public:
    int total_sales;
    float latest_amount;
    float total_sales_amount;
    String latest_timestamp;

    SalesData() : total_sales(0), latest_amount(0.0), total_sales_amount(0.0), latest_timestamp("") {}

    void updateSalesData(const String& serverUrl) {
        HTTPClient http;
        http.begin(serverUrl);  // Begin HTTP request
        int httpCode = http.GET();  // Send GET request

        if (httpCode == 200) {  // Check if response is OK
            String payload = http.getString();
            DynamicJsonDocument doc(2048);  // Create JSON document with sufficient capacity

            // Deserialize JSON payload
            DeserializationError error = deserializeJson(doc, payload);
            if (error) {
                Serial.print(F("Error parsing JSON: "));
                Serial.println(error.f_str());
                return;  // Return early if JSON parsing fails
            }

            // Reset totals and calculate fresh values
            total_sales = doc["sales"].size();
            total_sales_amount = 0.0;
            latest_amount = doc["sales"][0]["amount"].as<float>();
            latest_timestamp = doc["sales"][0]["timestamp"].as<String>();

            // Calculate total sales amount
            for (JsonObject sale : doc["sales"].as<JsonArray>()) {
                total_sales_amount += sale["amount"].as<float>();
            }

            // Update TFT display with new data
            updateSalesCount(total_sales);
            updateLatestAmount(latest_amount);
            updateTotalSalesAmount(total_sales_amount);

        } else {
            Serial.print(F("HTTP GET failed, error code: "));
            Serial.println(httpCode);
        }

        http.end();  // End the HTTP request
    }

    // Method to update the "Total Sales" count card
    void updateSalesCount(int sales) {
        tft.fillRect(10, TFT_HEIGHT * 0.17, TFT_WIDTH / 2 - 15, TFT_HEIGHT * 0.2, BACKGROUND_COLOR);
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%d", sales);
        drawCard(10, TFT_HEIGHT * 0.17, TFT_WIDTH / 2 - 15, TFT_HEIGHT * 0.2, CARD_COLOR, "Total Sales", buffer);
    }

    // Method to update the "Latest Sale" card
    void updateLatestAmount(float amount) {
        tft.fillRect(TFT_WIDTH / 2 + 5, TFT_HEIGHT * 0.17, TFT_WIDTH / 2 - 15, TFT_HEIGHT * 0.2, BACKGROUND_COLOR);
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "$%.2f", amount);
        drawCard(TFT_WIDTH / 2 + 5, TFT_HEIGHT * 0.17, TFT_WIDTH / 2 - 15, TFT_HEIGHT * 0.2, CARD_COLOR, "Latest Sale", buffer);
    }

    // Method to update the "Total Sales Amount" card
    void updateTotalSalesAmount(float amount) {
        tft.fillRect(10, TFT_HEIGHT * 0.4, TFT_WIDTH - 20, TFT_HEIGHT * 0.3, BACKGROUND_COLOR);
        char buffer[15];
        snprintf(buffer, sizeof(buffer), "$%.2f", amount);
        drawCard(10, TFT_HEIGHT * 0.4, TFT_WIDTH - 20, TFT_HEIGHT * 0.3, CARD_COLOR, "Total Sales ($)", buffer);
    }

    // Public method for calling drawCard
    void drawCard(int x, int y, int w, int h, uint16_t color, const char* title, const char* value) {
        tft.fillRoundRect(x, y, w, h, 10, color);
        tft.drawRoundRect(x, y, w, h, 10, CARD_BORDER_COLOR);  // Adding border
        tft.setTextColor(TEXT_COLOR);
        tft.setTextSize(1);
        tft.setCursor(x + 10, y + 5);
        tft.print(title);

        tft.setTextSize(2);
        tft.setCursor(x + 10, y + 25);
        tft.setTextColor(SUBTITLE_COLOR);
        tft.print(value);  // Value in lighter color for contrast
    }
};

#endif
