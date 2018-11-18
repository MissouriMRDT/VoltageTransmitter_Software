//trigBoard OpenWeather Project "Clear Skies"
// 10/26/18  Kevin Darrah
// RELEASED

// Includes - may not all be needed
#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <EEPROM.h>
#include <Servo.h>

Servo myservo;

int sleepHours = 3;

// PINS!!!
#define LEDpin 0
#define DONEpin 15
#define EXTWAKEpin 16 

//FUNCTIONS
void getWeather();
void saveConfigCallback ();
void readFile();
boolean WiFiConnection();
void checkTrigger();
void startOTA();
void longPress();

//globals for credentials
char zipCode[10];
char apiKey[40];

String weatherCondition = "";

//flag for saving data
bool shouldSaveConfig = false;

char batteryBuf[5] = "";// used to convert float to a char array
boolean externalWake = false;// to figure out who woke up the board
float batteryVoltage = 0;
float batteryThreshold = 3.3;

void setup() {

  Serial.begin(115200);//debug
  checkTrigger();
  if (batteryVoltage >= batteryThreshold) {
    readFile();
    WiFiConnection();
    getWeather();
  }
}

void loop() {
  digitalWrite(DONEpin, HIGH);
  delay(100);
  digitalWrite(DONEpin, LOW);
  delay(100);
}
