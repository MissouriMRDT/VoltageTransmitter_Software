//Wifi Connection Test
//Uses WFi_STA mode
//11-17-18


#include <ESP8266WiFi.h>

const char* ssid     = "MST-PSK-N";
const char* password = "JoeMiner";

void setup() {
  Serial.begin(115200);
  delay(10);
  int numberOfNetworks = WiFi.scanNetworks();
  Serial.println("Visible Networks:");
  for(int i =0; i<numberOfNetworks; i++){
 
      Serial.print("Network name: ");
      Serial.print(WiFi.SSID(i));
      Serial.print(", ");
      Serial.println(WiFi.RSSI(i)); 
  }
  
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  int count = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected"); 
  Serial.print("Connection Time: ");
  Serial.print(millis()-count);
  Serial.println("ms");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
}

