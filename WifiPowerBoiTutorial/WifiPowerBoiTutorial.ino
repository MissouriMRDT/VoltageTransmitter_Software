// Voltage Transmitter Software //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Main ino File
//
// Created for 2019 Valkyrie by: Eli Verbrugge, Ethan Arneson, Jacob Lipina
// 
// ESP8266-12e device plugs into battery pack and relays the measured voltage to the "#batteryvoltage" Slack channel. 
//
//Good Sources:
//General info and Pinout: https://github.com/nodemcu/nodemcu-devkit-v1.0
//https://forum.arduino.cc/index.php?topic=155218.0
//https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/BearSSL_Validation/BearSSL_Validation.ino is being used for the fetchInsecure() and other things.

// Libraries ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <ESP8266WiFi.h>
#include <sstream>
#include <ESPInsecure.h>

// Variable Constants & Definitions /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BUTTON_PIN          0
#define VOLT_MEAS_PIN       A0
#define SEND_MSG_PIN        16 //Pin labeled "D0" is GPIO16
#define PACK_VOLTS_MAX      33600 // mvolts
#define PACK_VOLTS_MIN      21600 // mvolts
#define PACK_DISCONNECTED   5 // volts
#define PACK_VOLTS_ADC_MAX  967
#define PACK_VOLTS_ADC_MIN  629 //Everything before this line is specific to Voltage Transmitter, the ADC min/max need to be calibrated for every different ESP8266 we use.
#define PACK_NAME           "Atlas Pack _Main_" // Choices: "Valkyrie Pack _Dewey_" ; "Valkyrie Pack _TBD_" ; "Atlas Pack _Main_" ; "Atlas Pack _Spare_" ; "Gryphon Pack _Main_" 
#define PACK_LOGO           ":atlas_logo_short: " // Choices: ":dew: " for Valkyrie Pack Dewey; TBD for Valkyrie Pack 2; ":atlas_logo_short: " for Atlas Pack Main; ":shimble: " for Atlas Pack Spare; ":gryphon_logo3: " for Gryphon Pack Main
int num_loops               = 0; //num times program has looped
int adc_reading             = 0;
float measured_voltage      = 0;
uint32_t send_msg_timer     = millis();
const uint32_t SEND_MSG     = 1800000; //30min = 1800000msec 
uint32_t num_disconnected   = 0; 

std::string message         = ""; //the actual message to be sent

BearSSL::WiFiClientSecure client; //We need to instantiate the wifi client here and pass it by reference for the writes to work.

// Functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() 
{
  Serial.begin(115200);

  pinMode(SEND_MSG_PIN, OUTPUT); 
  digitalWrite(SEND_MSG_PIN, LOW); //LED is active low
  

  WiFi.mode(WIFI_STA); // We start by connecting to a WiFi network
  WiFi.setSleepMode(WIFI_NONE_SLEEP); //Tell the ESP to never sleep, this needs to have a better solution researched.
  WiFi.begin(ssid, pass); //begin the connection to the wifi network.

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop()
{  
  if (send_msg_timer > millis())
  {
    send_msg_timer = millis();
  }
  if ((millis() - send_msg_timer > SEND_MSG) || num_loops == 0) //keep the num_loops == 0, which forces the loop to always run when the board starts.
  {
	//provide further control statements in the loop here
	  
    message = ":boi_matt2: ";
    message += PACK_NAME;
    message += " *TEST#######*"; //Construct your message here.
    //message += PACK_NAME;
    //message += " *TEST*"; 
    postInsecure(&client, message); //this function is the entry point to our ESPInsecure.h file, which will take care of all the sending mechanics
    num_disconnected ++; //I grabbed the part of VoltageTransmitter which handles disconnected transmissions
    send_msg_timer = millis(); 
    digitalWrite(SEND_MSG_PIN, HIGH); // Blink blue led to indicate msg sent
    delay(250);
    digitalWrite(SEND_MSG_PIN, LOW);
    
  }
  num_loops ++; //increment 
}

