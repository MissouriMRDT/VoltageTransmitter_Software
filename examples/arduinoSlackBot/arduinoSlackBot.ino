/**
 * ArduinoSlackBot.ino
 *
 *  Created on: 22.07.16
 *  By: Noah Shibley 
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <time.h>
#include <ArduinoJson.h>

#include <ESP8266HTTPClient.h>
#include <Hash.h>
#include <WebSocketsClient.h>
#include <ArduinoSlackBot.h>

const char* ssid = "MST-PSK-N";              //  your network SSID (name)
const char* pass = "JoeMiner";       // your network password
const char* botToken = ""; //your Slack bot token 
const char* botID = "@battery_voltage"; //your Slack bot user ID <@*********>

const int timezone = 9;     
const int dst = 0; //daylight savings

WiFiClient client;
ArduinoSlackBot bot;

/****************************************************/
void setup() {


  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  //Serial.println(ESP.getResetReason());
  delay(10);

  WiFi.begin(ssid, pass);
  
  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
  
  //setup network time protocol (ntp)
  configTime(timezone * 3600, dst, "pool.ntp.org", "time.nist.gov"); //configtime is esp8266 function
  
  Serial.println("\nWaiting for time sync");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Synced");

  delay(500);

  //bot.begin requires your bot token, and bot user id
  bot.begin(botToken,botID); 


  delay(500);
}

uint32_t timer = millis();
/****************************************************/
void loop() {
  if(timer > millis())
  {
	  timer = millis();
  }
  if(millis() - timer > 20000)
  {
	bot.update(); //check for messages and execute responses
	Serial.println("Sending message");
	const char* chan = "#batteryvoltage";
    const char* msg = "Testing";
    bot.sendMsg(chan, msg);
	char* data;
	timer = millis();
  }	
}

/****************************************************/
void botCommands() {

  //syntax for bot.hears(regex, reponse function). Try it with a C++11 lambda 
  bot.hears("hello|hi|yo|howdy",[](){ 
    bot.replyMsg("hey hey!"); 
  });

  
  //if the bot doesn't understand (match) reply with a default message
  bot.fails("Sorry I didn't get that.");

}







