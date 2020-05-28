 #include <brzo_i2c.h>        // Only needed for Arduino 1.6.5 and earlier
 #include <SSD1306BrzoMod.h>
 #include <ESP8266WiFi.h>
 #include <Variables.h>

 const char* ssid = WIFI_SSID;
 const char* pass = WIFI_PASS;
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
  display.setTextAlignment(TEXT_ALIGN_LEFT);

  display.drawStringMaxWidth(0, 0, 64, "Connecting to " + String(ssid));
  display.display();
  
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  display.clear();
  display.drawStringMaxWidth(0,0,64, "IP: " + WiFi.localIP().toString());
  display.display();
  delay(1500);

}

void drawTextAlignmentDemo() {
  //display.clear();
    // Text alignment demo
  display.setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
  
  display.drawString(0, 0, "Left");
}


void loop() {
  drawTextAlignmentDemo();
  //display.display();
  delay(100);
}