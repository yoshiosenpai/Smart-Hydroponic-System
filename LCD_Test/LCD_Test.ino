/*
 * 16x2 I2C LCD Test - ESP32
 *
 * Tests the LCD display only. No other hardware needed.
 * Wiring: LCD VCC→5V, GND→GND, SDA→GPIO21, SCL→GPIO22
 *
 * If nothing appears: try changing LCD_ADDR to 0x3F
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Same as main project - change to 0x3F if your LCD uses that address
#define LCD_ADDR  0x27
#define LCD_COLS  16
#define LCD_ROWS  2

LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS);

void setup() {
  Serial.begin(115200);
  delay(500);

  lcd.init();
  lcd.backlight();

  // Row 0
  lcd.setCursor(0, 0);
  lcd.print("LCD Test OK!");
  // Row 1
  lcd.setCursor(0, 1);
  lcd.print("Row 2 works!");

  Serial.println("LCD test started. You should see text on the screen.");
}

void loop() {
  // Blink backlight every 2 seconds to show the sketch is running
  static unsigned long last = 0;
  static bool on = true;
  if (millis() - last >= 2000) {
    last = millis();
    on = !on;
    if (on) {
      lcd.backlight();
    } else {
      lcd.noBacklight();
    }
  }
  delay(100);
}
