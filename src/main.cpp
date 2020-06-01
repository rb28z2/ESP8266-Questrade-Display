 #include <brzo_i2c.h>        // Only needed for Arduino 1.6.5 and earlier
 #include <SSD1306BrzoMod.h>
 #include <ESP8266WiFi.h>
 #include <Variables.h>
 #include <PubSubClient.h>

typedef void (*Page)(void);

 const char* ssid = WIFI_SSID;
 const char* pass = WIFI_PASS;
 const char* subscribe_topic = SUBSCRIBE_TOPIC;
 const char* mqtt_user = MQTT_USER;
 const char* mqtt_pass = MQTT_PASS;
// Initialize the OLED display using Arduino Wire:
SSD1306BrzoMod display(0x3c, SDA, SCL, GEOMETRY_64_48);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically 

WiFiClient espClient;
PubSubClient mqttClient(espClient);

String totalValue = "----.--";
String openPNL = "----.--";
String dailyPNL = "----.--";
String dailyPNLPercent = "---.--";

void callback(char* topic, byte* payload, unsigned int length)
{
  payload[length] = '\0';
  String strTopic = String((char*) topic);
  //Serial.println(strTopic);

  String payloadString = String((char*) payload);

  String marketValueTopic = String(subscribe_topic) + "totalMarketValue";
  String openPNLTopic = String(subscribe_topic) + "openPNL";
  String dailyPNLTopic = String(subscribe_topic) + "dailyPNL";
  String dailyPNLPercentTopic = String(subscribe_topic) + "dailyPNLPercent";

  
  if (strTopic == marketValueTopic) {
     totalValue = payloadString;
  } else if (strTopic == openPNLTopic) {
    openPNL = payloadString;
  } else if (strTopic == dailyPNLTopic) {
    dailyPNL = payloadString;
  } else if (strTopic == dailyPNLPercentTopic) {
    dailyPNLPercent = payloadString;
  }
}

void reconnect() {
  while (!mqttClient.connected())
  {
    if (mqttClient.connect("Questrade-Display", mqtt_user, mqtt_pass))
    {
      Serial.println("Connected to MQTT Server");
      String topic = String(subscribe_topic) + "#";
      mqttClient.subscribe(topic.c_str());
    }
    else
    {
      Serial.print("Connection to MQTT Server failed. Reason: ");
      Serial.print(mqttClient.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println();

  Serial.println("Starting");

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

  display.clear();
  display.drawStringMaxWidth(0,0,64, "Connecting to MQTT");
  display.display();

  mqttClient.setServer("192.168.3.2", 1883);
  mqttClient.setCallback(callback);
}

void displayTotalValue(){
  display.clear();
  display.drawStringMaxWidth(0,0,64, "Total Value:");
  display.drawStringMaxWidth(0,11,64, "$" + totalValue);
  display.drawStringMaxWidth(0,25,64, "Open PNL:");
  display.drawStringMaxWidth(0,36,64, "$" + openPNL);
  display.display();
}

void displayDailyValue(){
  display.clear();
  display.drawStringMaxWidth(0,0,64, "Daily PNL:");
  display.drawStringMaxWidth(0,11,64, "$" + dailyPNL);
  display.drawStringMaxWidth(0,25,64, "Daily PNL %");
  display.drawStringMaxWidth(0,36,64, dailyPNLPercent + "%");
  display.display();
}

long timeSinceLastSwitch = 0;
Page pages[] = {displayTotalValue, displayDailyValue};
int pageLength = (sizeof(pages) / sizeof(Page));
int page = 0;

void loop() {

  if (!mqttClient.connected()){
    Serial.println("Reconnecting to MQTT");
    reconnect();
  }

  mqttClient.loop();
  
  pages[page]();
  
  if (millis() - timeSinceLastSwitch > 10 * 1000){
    page = (page + 1) % pageLength;
    timeSinceLastSwitch = millis();
  }
}