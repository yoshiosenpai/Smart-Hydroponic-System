# Smart Plant Monitoring & Protection System

## Project Overview

A wireless plant monitoring and protection system that monitors temperature and humidity, detects animal activity, and allows remote control of a water pump for nutrient supply. All data is sent to the Blynk mobile app for real-time monitoring, instant motion alerts, and remote pump control.

---

## System Features

| Feature | Description |
|---------|-------------|
| **Temperature & Humidity** | DHT22 sensor monitors environmental conditions continuously |
| **Animal Detection** | PIR motion sensor detects animal activity to protect plants |
| **Instant Alerts** | Push notifications sent to Blynk app when motion is detected; red indicator shown |
| **Water Pump Control** | Turn pump on/off remotely via Blynk app for nutrient supply |
| **Local Display** | 16×2 LCD shows temperature, humidity, motion status, and pump state |
| **Wireless Connectivity** | ESP32 WiFi module enables cloud connectivity and remote access |

---

## Hardware Components

| Component | Quantity | Specification |
|-----------|----------|---------------|
| **ESP32 Expansion Board** | 1 | Main microcontroller with built-in WiFi |
| **DHT22** | 1 | Temperature & humidity sensor |
| **PIR Motion Sensor** | 1 | HC-SR501 or similar (3–7m range) |
| **5V Relay Module** | 1 | Single-channel relay for pump control |
| **DC Water Pump** | 1 | 5–12V submersible or inline pump |
| **16×2 LCD** | 1 | I2C interface (e.g., PCF8574 backpack) |
| **18650 Battery** | 2 | Power source (with holder) |
| **Battery Holder** | 1 | 2S configuration for 7.4V |
| **Jumper Wires** | — | Male-to-male, male-to-female |
| **Breadboard** | 1 | Optional, for prototyping |

### Power Notes

- ESP32 Expansion Board: 3.3V logic, 5V input via USB or Vin
- DHT22, PIR, Relay: 5V or 3.3V (check module specs)
- Water pump: Use relay; power pump from battery/separate supply, not directly from ESP32
- If using 18650 batteries (2S ≈ 7.4V), use a 5V/3.3V step-down regulator for ESP32 and sensors

---

## Wiring Diagram

> **Full wiring guide:** See [WIRING_DIAGRAM.md](WIRING_DIAGRAM.md) for a complete schematic with component-by-component connections.

### ESP32 Pin Connections

| Component | ESP32 GPIO | Pin Function |
|-----------|------------|--------------|
| DHT22 Data | GPIO 15 | Data (single-wire) |
| PIR Output | GPIO 4 | Digital input |
| Relay Control | GPIO 2 | Digital output |
| LCD SDA | GPIO 21 | I2C Data |
| LCD SCL | GPIO 22 | I2C Clock |

### Connection Details

**DHT22:**
- VCC → 3.3V (or 5V if module supports)
- GND → GND
- DATA → GPIO 15
- (Add 4.7kΩ–10kΩ pull-up between DATA and VCC if not on module)

**PIR Motion Sensor:**
- VCC → 5V (or 3.3V)
- GND → GND
- OUT → GPIO 4

**5V Relay:**
- VCC → 5V
- GND → GND
- IN → GPIO 2  
- **COM, NO, NC** → Connect pump power through COM and NO (Normally Open) for relay-controlled switching

**16×2 I2C LCD:**
- VCC → 5V (or 3.3V)
- GND → GND
- SDA → GPIO 21
- SCL → GPIO 22

**Water Pump:**
- One wire → Relay COM
- Other wire → Relay NO  
- Power the pump from battery/external supply through the relay contacts (do not power pump directly from ESP32)

---

## Software Requirements

### 1. Arduino IDE

- **Download:** https://www.arduino.cc/en/software
- Install the macOS version

### 2. ESP32 Board Support

1. File → Preferences
2. In **Additional Board Manager URLs**, add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Tools → Board → Boards Manager
4. Search **“esp32”** and install **ESP32 by Espressif Systems**
5. Tools → Board → Select your board (e.g. **ESP32 Dev Module** or your specific expansion board)

### 3. Required Libraries

Install via **Sketch → Include Library → Manage Libraries**:

| Library | Search Term | Author |
|---------|-------------|--------|
| Blynk | Blynk | Volodymyr Shymanskyy |
| DHT sensor library | DHT sensor | Adafruit |
| Adafruit Unified Sensor | Adafruit Unified Sensor | Adafruit |
| LiquidCrystal I2C | LiquidCrystal I2C | Frank de Brabander |

### 4. USB-Serial Driver (if needed)

If the ESP32 is not detected:

- **CP210x:** https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers  
- **CH340:** `brew install wch-ch34x-usb-serial-driver`

---

## Blynk App Setup

### 1. Create Blynk Account

1. Go to https://blynk.cloud/ (or use the Blynk IoT app)
2. Sign up / log in

### 2. Create Template

1. **Developer Zone** → **Templates** → **New Template**
2. **Template Name:** `Smart Plant Monitor`
3. **Hardware:** ESP32
4. **Connection Type:** WiFi

### 3. Create Datastreams

Add these datastreams to the template:

| Name | Type | Data Type | Virtual Pin | Min | Max | Notes |
|------|------|-----------|-------------|-----|-----|-------|
| Temperature | Virtual Pin | Double | V0 | 0 | 100 | °C |
| Humidity | Virtual Pin | Double | V1 | 0 | 100 | % |
| Pump Switch | Virtual Pin | Integer | V2 | 0 | 1 | 0=OFF, 1=ON |
| Motion Status | Virtual Pin | Integer | V3 | 0 | 1 | 0=No, 1=Yes (red) |
| Motion Alert | Virtual Pin | Integer | V4 | 0 | 1 | Trigger for notification |

### 4. Create Event (Push Notification)

1. In your Template → **Events** → **Add Event**
2. **Event Code:** `motion_alert`
3. Enable **Push Notifications**
4. Add recipients (users who will receive alerts)
5. Message: e.g. “Animal/Intruder detected! Check your plants.”

### 5. Create Widgets in Template

| Widget | Datastream | Configuration |
|--------|------------|---------------|
| **Labeled Value** | Temperature (V0) | Label: "Temperature", Suffix: "°C" |
| **Labeled Value** | Humidity (V1) | Label: "Humidity", Suffix: "%" |
| **Button/Switch** | Pump Switch (V2) | Label: "Water Pump", Mode: Switch |
| **LED/Value Display** | Motion Status (V3) | Label: "Motion", Color: Red when 1 |
| **Chart** (optional) | V0, V1 | For temperature/humidity history |

### 6. Create Device and Get Credentials

1. **Devices** → **Add Device** → Select your template
2. Open the device → **Developer Tools**
3. Copy **Template ID**, **Template Name**, and **Auth Token**

---

## Code Configuration

Before uploading, update these values in `Smart_Plant_Monitor.ino`:

```cpp
// Blynk credentials (from Blynk Cloud)
#define BLYNK_TEMPLATE_ID     "your_template_id_here"
#define BLYNK_TEMPLATE_NAME   "Smart Plant Monitor"
#define BLYNK_AUTH_TOKEN      "your_auth_token_here"

// WiFi credentials
char ssid[] = "Your_WiFi_Network_Name";
char pass[] = "Your_WiFi_Password";
```

### Optional: LCD I2C Address

If the LCD does not display, its I2C address may differ. Common values:
- `0x27` (default in code)
- `0x3F`

Change `LCD_ADDR` in the code if needed. Use an I2C scanner sketch to find the correct address.

---

## Upload and Run

1. Connect ESP32 to Mac via USB
2. Tools → Port → Select the ESP32 port (e.g. `/dev/cu.usbserial-xxxx`)
3. Tools → Board → ESP32 Dev Module (or your board)
4. File → Open → `Smart_Plant_Monitor.ino`
5. Update Blynk and WiFi credentials
6. Sketch → Upload
7. Open **Tools → Serial Monitor** (115200 baud) to verify output

---

## Usage

### Local Display (LCD)

- **Row 1:** Temperature (°C) and Humidity (%)
- **Row 2:** Motion status (YES/NO) and Pump status (ON/OFF)

### Blynk App

- **Temperature & Humidity:** Live values
- **Water Pump:** Switch to turn pump ON/OFF
- **Motion Indicator:** Red when motion detected
- **Notifications:** Push alert when motion is detected

### PIR Warm-up

The PIR needs ~30–60 seconds after power-on to stabilize. Avoid moving during this period for accurate detection.

---

## Troubleshooting

| Problem | Possible Cause | Solution |
|---------|----------------|----------|
| No WiFi connection | Wrong SSID/password | Recheck credentials; verify 2.4 GHz network |
| Blynk not connecting | Wrong template/token | Verify Template ID, Name, Auth Token |
| No LCD output | Wrong I2C address | Try 0x27 or 0x3F; run I2C scanner |
| DHT reads NaN | Loose wiring / wrong pin | Check DATA, VCC, GND; ensure pull-up resistor |
| PIR always HIGH | Warm-up not complete | Power on, wait 1 minute |
| Pump not switching | Relay wiring | Check COM/NO; power pump via relay contacts |
| Motion no alert | Event not configured | Create `motion_alert` event; enable push notifications |

### Serial Monitor

Use the Serial Monitor at 115200 baud to see:
- Temperature and humidity readings
- Pump on/off messages
- Motion detection events
- Connection status

---

## Block Diagram

```
                    ┌─────────────────────────────────────┐
                    │              ESP32                  │
                    │                                     │
  DHT22 ────────────►│ GPIO15  Temperature & Humidity      │
                    │                                     │
  PIR ─────────────►│ GPIO4   Motion Detection            │──────► Blynk Cloud
                    │                                     │             │
  Relay ◄───────────│ GPIO2   Pump Control                │             │
                    │                                     │             ▼
  LCD ◄────────────►│ GPIO21/22  I2C Display              │       Blynk App
                    │                                     │       - Live data
                    └─────────────────────────────────────┘       - Alerts
                                                                 - Pump control
```

---

## Project Structure

```
Smart_Plant_Monitor/
├── Smart_Plant_Monitor.ino   # Main Arduino sketch
└── README.md                 # This documentation
```

---

## License & Credits

- **DHT Library:** Adafruit
- **Blynk:** Blynk IoT Platform
- **LiquidCrystal I2C:** Frank de Brabander

---

*Smart Plant Monitoring & Protection System — FYP Project*
