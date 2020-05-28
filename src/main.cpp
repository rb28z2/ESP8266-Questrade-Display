//#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
//#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"
// OR #include "SH1106Wire.h"   // legacy: #include "SH1106.h"

// For a connection via I2C using brzo_i2c (must be installed) include:
 #include <brzo_i2c.h>        // Only needed for Arduino 1.6.5 and earlier
 #include <SSD1306BrzoMod.h>
// OR #include "SH1106Brzo.h"

// For a connection via SPI include:
// #include <SPI.h>             // Only needed for Arduino 1.6.5 and earlier
// #include "SSD1306Spi.h"
// OR #include "SH1106SPi.h"


// Initialize the OLED display using Arduino Wire:
SSD1306BrzoMod display(0x3c, SDA, SCL, GEOMETRY_64_48);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically 


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();


  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  
}

void drawTextAlignmentDemo() {
    // Text alignment demo
  display.setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "Left");
}


void loop() {
  drawTextAlignmentDemo();
  display.display();
  delay(100);
}