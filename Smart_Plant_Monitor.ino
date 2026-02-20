/*
 * Smart Plant Monitoring & Protection System
 * 
 * Features:
 * - DHT22: Temperature & Humidity monitoring
 * - PIR Motion Sensor: Animal activity detection with Blynk alerts
 * - Water Pump: On/Off control via Blynk app (5V relay)
 * - 16x2 LCD: Local data display
 * - ESP32: WiFi connectivity for Blynk cloud
 * 
 * Hardware: ESP32 Expansion Board, DHT22, PIR, 5V Relay, 16x2 I2C LCD
 */
// ============ WIFI CREDENTIALS ============
char ssid[] = "4Cam";
char pass[] = "a123456??";

#define BLYNK_TEMPLATE_ID "TMPL6bEh1Nf2E"
#define BLYNK_TEMPLATE_NAME "Smart Hydroponic"
#define BLYNK_AUTH_TOKEN      "VlJbimOaBnpHHfDeHDTkdk2JQmkdv4KV"       // Get from Blynk Cloud

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>



// ============ PIN DEFINITIONS (ESP32) ============
#define DHT_PIN      15      // DHT22 data (GPIO15)
#define PIR_PIN      4       // PIR motion sensor output (GPIO4)
#define RELAY_PIN    2       // Relay control for water pump (GPIO2)
#define DHT_TYPE     DHT22

// I2C LCD: SDA=GPIO21, SCL=GPIO22 (default ESP32 I2C pins)
#define LCD_ADDR     0x27    // Common I2C address for 16x2 LCD (try 0x3F if not working)
#define LCD_COLS     16
#define LCD_ROWS     2

// ============ OBJECTS ============
DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

// ============ BLYNK VIRTUAL PINS ============
#define V_TEMPERATURE     V0   // Temperature (ºC)
#define V_HUMIDITY        V1   // Humidity (%)
#define V_PUMP_SWITCH     V2   // Water pump on/off
#define V_MOTION_STATUS   V3   // Motion detected (0=no, 1=yes - shows red)
#define V_MOTION_ALERT    V4   // Trigger for push notification

// ============ VARIABLES ============
float temperature = 0.0;
float humidity = 0.0;
bool motionDetected = false;
bool lastMotionState = false;
bool pumpState = false;
unsigned long lastDHTRead = 0;
unsigned long lastLCDUpdate = 0;
const unsigned long DHT_INTERVAL = 2000;   // Read DHT every 2 seconds
const unsigned long LCD_INTERVAL = 1000;   // Update LCD every 1 second

// ============ BLYNK HANDLERS ============

// Water pump switch - control from Blynk app
BLYNK_WRITE(V_PUMP_SWITCH) {
  pumpState = param.asInt();
  // ACTIVE LOW relay: LOW = ON, HIGH = OFF (inverted logic)
  digitalWrite(RELAY_PIN, pumpState ? LOW : HIGH);
  Serial.println(pumpState ? "Pump: ON" : "Pump: OFF");
}

// ============ SETUP ============
void setup() {
  Serial.begin(115200);
  delay(500);  // Give serial time to start
  
  // Initialize pins
  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Pump OFF by default (ACTIVE LOW relay: HIGH = OFF)
  
  // Initialize DHT22
  dht.begin();
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Plant");
  lcd.setCursor(0, 1);
  lcd.print("WiFi...");
  
  // Connect WiFi FIRST (required before Blynk)
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  lcd.setCursor(0, 1);
  lcd.print("Blynk...     ");
  
  // Connect to Blynk (after WiFi is ready)
  Blynk.config(BLYNK_AUTH_TOKEN, "blynk.cloud", 80);
  Blynk.connect();
  
  lcd.setCursor(0, 1);
  lcd.print("Ready!       ");
  
  Serial.println("=== Smart Plant Monitor Started ===");
}

// ============ MAIN LOOP ============
void loop() {
  Blynk.run();
  yield();  // Allow WiFi/Blynk tasks to run
  
  unsigned long now = millis();
  
  // Read DHT22 periodically
  if (now - lastDHTRead >= DHT_INTERVAL) {
    readDHT();
    lastDHTRead = now;
  }
  
  // Check PIR motion sensor
  checkMotion();
  
  // Update LCD display
  if (now - lastLCDUpdate >= LCD_INTERVAL) {
    updateLCD();
    lastLCDUpdate = now;
  }
  
  delay(10);  // Prevent tight loop, allow FreeRTOS tasks to run
}

// ============ FUNCTIONS ============

void readDHT() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  
  if (!isnan(t) && !isnan(h)) {
    temperature = t;
    humidity = h;
    
    // Send to Blynk (only when connected)
    if (Blynk.connected()) {
      Blynk.virtualWrite(V_TEMPERATURE, temperature);
      Blynk.virtualWrite(V_HUMIDITY, humidity);
    }
    
    Serial.printf("Temp: %.1f°C | Hum: %.1f%%\n", temperature, humidity);
  }
}

void checkMotion() {
  motionDetected = (digitalRead(PIR_PIN) == HIGH);
  
  // Rising edge - motion just detected
  if (motionDetected && !lastMotionState) {
    Serial.println("*** MOTION DETECTED - Animal/Intruder Alert! ***");
    
    if (Blynk.connected()) {
      Blynk.virtualWrite(V_MOTION_STATUS, 1);
      Blynk.logEvent("motion_alert", "Animal/Intruder detected! Check your plants.");
      Blynk.virtualWrite(V_MOTION_ALERT, 1);
    }
  }
  
  // Falling edge - motion cleared
  if (!motionDetected && lastMotionState) {
    if (Blynk.connected()) {
      Blynk.virtualWrite(V_MOTION_STATUS, 0);
    }
    Serial.println("Motion cleared.");
  }
  
  lastMotionState = motionDetected;
}

void updateLCD() {
  lcd.clear();
  
  // Row 0: Temperature and Humidity
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print("C H:");
  lcd.print(humidity, 0);
  lcd.print("%");
  
  // Row 1: Motion status and Pump status
  lcd.setCursor(0, 1);
  lcd.print("Motion:");
  lcd.print(motionDetected ? "YES" : "NO ");
  lcd.print(" P:");
  lcd.print(pumpState ? "ON" : "OFF");
}
