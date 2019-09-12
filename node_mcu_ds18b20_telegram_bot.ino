#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <TelegramBot.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port D2 on the ESP8266
#define ONE_WIRE_BUS D2
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

float tempSensor;
// unique DS18b20 id
uint8_t sensor[8] = { 0x28, 0xFF, 0x95, 0x36, 0x81, 0x15, 0x01, 0xE8 };

const char* ssid = "xxxxxxxxxxxx";
const char* password = "xxxxxxxxxxxx";
const char BotToken[] = "xxxxxxxxxxxx";

WiFiClientSecure net_ssl;
TelegramBot bot (BotToken, net_ssl);

void setup() {

  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  bot.begin();

  pinMode(LED, OUTPUT);
}

void loop() {
  message m = bot.getUpdates();
  if (m.text.equals("temp")) {
    sensors.requestTemperatures();
    tempSensor = sensors.getTempC(sensor);
    bot.sendMessage(m.chat_id, String(tempSensor) + "°C");
  } else if (m.text.equals("/start")) {
    bot.sendMessage(m.chat_id, "Yo, to get temperature send me 'temp'");
  }
}
