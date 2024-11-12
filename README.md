# Creem Sales Monitor

A simple monitor to display creem sales on an ESP32 using a Lilygo screen.

## Features:
- Displays total sales, latest sale, and total sales amount
- Connects to your server to fetch sales data via HTTP request
- Displays sales data on a TFT screen
- Adds and saves new sales to a JSON file on the server

## Requirements:
- Arduino IDE
- ESP32 Lilygo T-Display-S3
- Libraries: WiFi, HTTPClient, ArduinoJson, TFT_eSPI

## Setup:
1. Install Arduino IDE:
   Download and install the Arduino IDE.

2. Install ESP32 in Arduino IDE:
   Open Arduino IDE > File > Preferences.
   Add this URL to Additional Boards Manager:  
   `https://dl.espressif.com/dl/package_esp32_index.json`

   Go to Tools > Board > Boards Manager and install ESP32 by Espressif.

3. Select ESP32 Board:
   Go to Tools > Board and select your ESP32 board (e.g., ESP32 Dev Module).
   Select the correct Port under Tools > Port.

4. Modify Configuration:
   Open `config.h` and set:

   ```cpp
   #define WIFI_SSID     "your-SSID"
   #define WIFI_PASSWORD "your-PASSWORD"
   #define SERVER_URL "http://your-server-url.com/api/sales"
   #define TITLE "XBlockBot Sales"
   ```
   
5. Upload Code: Open creem_monitoring.ino in Arduino IDE. Click Upload to compile and upload the code to ESP32.
   
Server Setup: To handle the sales data on the server, you'll need to create two API endpoints: one to fetch sales from a JSON file and one to store new sales.

Very important: Remember to add your endpoint to a Creem webhook

1. Endpoint to Fetch Sales Data
You can create an endpoint to fetch sales data from a JSON file like this:

```js
// /api/sales/
import { NextResponse } from "next/server";
import { promises as fs } from 'fs';

const SALES_FILE_PATH = './public/sales.json';

export async function GET() {
  try {
    // Read the sales file
    const fileContent = await fs.readFile(SALES_FILE_PATH, 'utf-8');
    const sales = JSON.parse(fileContent);
    
    return NextResponse.json({ sales }, { status: 200 });
  } catch (error) {
    console.error('Error reading sales:', error);
    return NextResponse.json(
      { error: "Error fetching sales data" },
      { status: 500 }
    );
  }
}
```

This endpoint will read from sales.json and return the sales data in JSON format. Ensure the path is correct based on your server's file structure.

2. Endpoint to Save New Sales Data
You can use the following code to store new sales to the sales.json file:

```js
// /api/sales/webhook
// Add this code to your existing webhook endpoint
const SALES_FILE_PATH = './public/sales.json';

async function storeSale(order) {
  const saleData = {
    timestamp: new Date().toISOString(),
    orderId: order.id,
    amount: order.amount,
  };

  try {
    // Read existing sales
    let sales = [];
    try {
      const fileContent = await fs.readFile(SALES_FILE_PATH, 'utf-8');
      sales = JSON.parse(fileContent);
    } catch (error) {
      // File doesn't exist yet, start with empty array
    }

    // Add new sale and write back to file
    sales.push(saleData);
    await fs.writeFile(SALES_FILE_PATH, JSON.stringify(sales, null, 2));
  } catch (error) {
    console.error('Error storing sale:', error);
  }
}
```

This function will read the existing sales data, add a new sale entry, and then write the updated data back to the sales.json file.

License:
MIT License
