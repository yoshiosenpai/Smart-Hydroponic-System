/*
 * Relay Module Test - ESP32 (Diagnostic Version)
 *
 * Tests the 5V relay module to determine if it's ACTIVE HIGH or ACTIVE LOW.
 * Wiring: Relay VCC→5V, GND→GND, IN→GPIO16
 *
 * If relay stays ON all the time, it's likely ACTIVE LOW.
 * This test will help you identify which type you have.
 */

#define RELAY_PIN  16   // Relay control pin (GPIO16)

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Start OFF (ACTIVE LOW: HIGH = OFF)

  Serial.println("Relay Test - GPIO16 (ACTIVE LOW)");
  Serial.println("ACTIVE LOW relay: LOW = ON, HIGH = OFF");
  Serial.println("Relay will toggle ON/OFF every 2 seconds");
  Serial.println("----------------------------------------");
}

void loop() {
  // Turn relay ON (ACTIVE LOW: LOW = ON)
  digitalWrite(RELAY_PIN, LOW);
  Serial.println(">>> RELAY ON (Pin = LOW)");
  delay(2000);  // Keep ON for 2 seconds

  // Turn relay OFF (ACTIVE LOW: HIGH = OFF)
  digitalWrite(RELAY_PIN, HIGH);
  Serial.println("RELAY OFF (Pin = HIGH)");
  delay(2000);  // Keep OFF for 2 seconds
}
