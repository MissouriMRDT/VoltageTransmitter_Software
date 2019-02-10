#include <ESP8266WiFi.h>

void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
   delay(500);
 
   Serial.println();
   Serial.print("MAC: ");
   Serial.println(WiFi.macAddress());
}

void loop() {
  // put your main code here, to run repeatedly:

}
