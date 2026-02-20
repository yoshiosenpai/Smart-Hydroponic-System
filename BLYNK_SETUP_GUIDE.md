# Blynk Setup Guide - Step by Step

Complete guide to set up Blynk Cloud for your Smart Plant Monitor project.

---

## Step 1: Create Blynk Account

1. Go to **https://blynk.cloud/** (or download **Blynk IoT** app on your phone)
2. Click **"Sign Up"** or **"Create Account"**
3. Enter your email: `s.mathavenkumar12@gmail.com`
4. Enter password: `Matha1212#`
5. Verify your email if required
6. Log in to your account

---

## Step 2: Create a Template

A template defines the structure of your device (datastreams, widgets, events).

1. After logging in, go to **"Developer Zone"** (top menu)
2. Click **"Templates"** → **"New Template"**
3. Fill in the form:
   - **Template Name:** `Smart Plant Monitor` (or `Smart Hydroponic`)
   - **Hardware:** Select **ESP32**
   - **Connection Type:** Select **WiFi**
   - **Template ID:** (auto-generated, you'll need this later)
4. Click **"Create"**

---

## Step 3: Create Datastreams

Datastreams are channels for data exchange between your ESP32 and Blynk app.

1. In your template, go to **"Datastreams"** tab
2. Click **"Add Datastream"** and create these 5 datastreams:

### Datastream 1: Temperature
- **Name:** `Temperature`
- **Type:** `Virtual Pin`
- **Data Type:** `Double`
- **Virtual Pin:** `V0`
- **Min:** `0`
- **Max:** `100`
- **Unit:** `°C`
- Click **"Save"**

### Datastream 2: Humidity
- **Name:** `Humidity`
- **Type:** `Virtual Pin`
- **Data Type:** `Double`
- **Virtual Pin:** `V1`
- **Min:** `0`
- **Max:** `100`
- **Unit:** `%`
- Click **"Save"**

### Datastream 3: Pump Switch
- **Name:** `Pump Switch`
- **Type:** `Virtual Pin`
- **Data Type:** `Integer`
- **Virtual Pin:** `V2`
- **Min:** `0`
- **Max:** `1`
- **Default:** `0`
- Click **"Save"**

### Datastream 4: Motion Status
- **Name:** `Motion Status`
- **Type:** `Virtual Pin`
- **Data Type:** `Integer`
- **Virtual Pin:** `V3`
- **Min:** `0`
- **Max:** `1`
- **Default:** `0`
- Click **"Save"**

### Datastream 5: Motion Alert
- **Name:** `Motion Alert`
- **Type:** `Virtual Pin`
- **Data Type:** `Integer`
- **Virtual Pin:** `V4`
- **Min:** `0`
- **Max:** `1`
- Click **"Save"**

---

## Step 4: Create Event (Push Notification)

Events trigger notifications when certain conditions occur.

1. In your template, go to **Developer Zone** → **"Events & Notifications"** tab (bell icon)
2. Click **"Add Event"**
3. Fill in:
   - **Event Code:** `motion_alert` (must match code exactly)
   - **Event Name:** `Motion Detected Alert`
   - **Description:** `Animal/Intruder detected near plants`
4. Scroll down to **"Notification Settings"**
5. Enable **"Push Notifications"**
6. Click **"Add Recipient"** → Select yourself (or add email/phone)
7. **Message Template:** `Animal/Intruder detected! Check your plants.`
8. Click **"Save"**

---

## Step 5: Create Widgets (Mobile App UI)

Widgets are the visual elements users see in the Blynk app.

1. In your template, go to **"Widgets"** tab
2. Click **"Add Widget"** and create these widgets:

### Widget 1: Temperature Display
- **Widget Type:** `Labeled Value` or `Super Chart`
- **Datastream:** Select `Temperature (V0)`
- **Label:** `Temperature`
- **Suffix:** `°C`
- **Color:** Blue (optional)
- Click **"Save"**

### Widget 2: Humidity Display
- **Widget Type:** `Labeled Value` or `Super Chart`
- **Datastream:** Select `Humidity (V1)`
- **Label:** `Humidity`
- **Suffix:** `%`
- **Color:** Green (optional)
- Click **"Save"**

### Widget 3: Water Pump Switch
- **Widget Type:** `Button` or `Switch`
- **Datastream:** Select `Pump Switch (V2)`
- **Label:** `Water Pump`
- **Mode:** `Switch` (ON/OFF toggle)
- **ON Label:** `ON`
- **OFF Label:** `OFF`
- **Color:** Blue (optional)
- Click **"Save"**

### Widget 4: Motion Indicator
- **Widget Type:** `LED` or `Value Display`
- **Datastream:** Select `Motion Status (V3)`
- **Label:** `Motion Detected`
- **ON Color:** Red
- **OFF Color:** Gray/Green
- Click **"Save"**

### Widget 5: Chart (Optional - for history)
- **Widget Type:** `Super Chart`
- **Datastreams:** Add both `Temperature (V0)` and `Humidity (V1)`
- **Label:** `Temperature & Humidity History`
- **Time Range:** `24 hours` or `7 days`
- Click **"Save"**

---

## Step 6: Create Device and Get Credentials

A device is an instance of your template that connects to Blynk Cloud.

1. Go to **"Devices"** (top menu)
2. Click **"Add Device"**
3. Select your template: **"Smart Plant Monitor"**
4. **Device Name:** `My Plant Monitor` (or any name)
5. Click **"Create"**
6. Click on your device to open it
7. Click the **"Developer Tools"** icon (wrench/tools icon)
8. You'll see three important values - **COPY THESE:**

```
Template ID: TMPLxxxxxxxxx
Template Name: Smart Plant Monitor
Auth Token: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
```

---

## Step 7: Update Your Arduino Code

1. Open `Smart_Plant_Monitor.ino` in Arduino IDE
2. Find these lines near the top:

```cpp
#define BLYNK_TEMPLATE_ID     "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME   "Smart Plant Monitor"
#define BLYNK_AUTH_TOKEN      "YOUR_AUTH_TOKEN"
```

3. Replace with your actual values from Step 6:

```cpp
#define BLYNK_TEMPLATE_ID     "TMPLxxxxxxxxx"        // Paste your Template ID
#define BLYNK_TEMPLATE_NAME   "Smart Plant Monitor"  // Your Template Name
#define BLYNK_AUTH_TOKEN      "your_auth_token_here"  // Paste your Auth Token
```

4. Also update WiFi credentials:

```cpp
char ssid[] = "Your_WiFi_Network_Name";
char pass[] = "Your_WiFi_Password";
```

5. **Save** the file (Ctrl+S / Cmd+S)

---

## Step 8: Upload Code to ESP32

1. Connect ESP32 to your Mac via USB
2. In Arduino IDE:
   - **Tools** → **Board** → Select your ESP32 board (e.g., **ESP32 Dev Module**)
   - **Tools** → **Port** → Select the COM port (e.g., `/dev/cu.usbserial-xxxx`)
3. Click **Upload** (→ button) or **Sketch** → **Upload**
4. Wait for upload to complete
5. Open **Serial Monitor** (Tools → Serial Monitor) at **115200** baud
6. You should see:
   ```
   WiFi connected
   === Smart Plant Monitor Started ===
   ```

---

## Step 9: Test in Blynk App

1. **Download Blynk IoT app** on your phone:
   - iOS: App Store → Search "Blynk IoT"
   - Android: Google Play → Search "Blynk IoT"
2. **Log in** with the same account (`s.mathavenkumar12@gmail.com`)
3. The app should automatically show your device **"My Plant Monitor"**
4. Tap on it to open the dashboard
5. You should see:
   - Temperature and Humidity values updating
   - Water Pump switch (try turning it ON/OFF)
   - Motion indicator (will turn red when motion detected)

---

## Troubleshooting

### Device Not Connecting

**Problem:** ESP32 not connecting to Blynk Cloud

**Solutions:**
- Check WiFi credentials in code (SSID and password)
- Verify ESP32 is connected to 2.4 GHz WiFi (not 5 GHz)
- Check Serial Monitor for error messages
- Verify Template ID, Template Name, and Auth Token are correct
- Ensure ESP32 and phone are on the same WiFi network (for local testing)

### No Data Showing

**Problem:** Widgets show "---" or no values

**Solutions:**
- Check Serial Monitor - is ESP32 connecting to WiFi?
- Verify datastream Virtual Pins match code (V0, V1, V2, V3, V4)
- Check device is online in Blynk Cloud dashboard
- Restart ESP32

### Push Notifications Not Working

**Problem:** No alert when motion detected

**Solutions:**
- Verify Event Code is exactly `motion_alert` (case-sensitive)
- Check Event has Push Notifications enabled
- Verify recipient is added in Event settings
- Check phone has notifications enabled for Blynk app
- Test event manually: In Events tab, click "Test" button

### Pump Switch Not Working

**Problem:** Can't control pump from app

**Solutions:**
- Verify Pump Switch widget is connected to V2 datastream
- Check relay wiring (GPIO 2)
- Check Serial Monitor for "Pump: ON" / "Pump: OFF" messages
- Verify relay module is powered (5V)

---

## Quick Reference: Virtual Pins

| Virtual Pin | Purpose | Direction | Data Type |
|-------------|---------|-----------|-----------|
| V0 | Temperature | ESP32 → App | Double |
| V1 | Humidity | ESP32 → App | Double |
| V2 | Pump Switch | App → ESP32 | Integer (0/1) |
| V3 | Motion Status | ESP32 → App | Integer (0/1) |
| V4 | Motion Alert | ESP32 → App | Integer (trigger) |

---

## Next Steps

Once everything is working:
1. Test all features (temperature, humidity, motion detection, pump control)
2. Customize widget colors and layout in Blynk app
3. Set up automations (optional): e.g., auto-turn pump ON when humidity < 30%
4. Share device access with others (if needed)

---

**Need Help?**
- Blynk Documentation: https://docs.blynk.io/
- Blynk Community: https://community.blynk.cc/
