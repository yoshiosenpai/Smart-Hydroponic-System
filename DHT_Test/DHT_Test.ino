/*
 * DHT22 Temperature & Humidity Test - ESP32
 *
 * Tests the DHT22 sensor only. No other hardware needed.
 * Wiring: DHT22 VCC→3.3V, GND→GND, DATA→GPIO15
 *         (Optional: 4.7kΩ–10kΩ pull-up between DATA and VCC)
 */

#include <DHT.h>

#define DHT_PIN   15   // Same as main project (GPIO15)
#define DHT_TYPE  DHT22

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(115200);
  delay(500);

  dht.begin();
  Serial.println("DHT22 Test - Reading temperature and humidity...");
  Serial.println("----------------------------------------");
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("DHT read failed! Check wiring (DATA→GPIO15, VCC, GND).");
  } else {
    Serial.print("Temperature: ");
    Serial.print(t, 1);
    Serial.print(" °C  |  Humidity: ");
    Serial.print(h, 1);
    Serial.println(" %");
  }

  delay(2000);  // Read every 2 seconds
}
