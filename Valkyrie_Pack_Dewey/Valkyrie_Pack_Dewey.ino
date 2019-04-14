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

// Variable Constants & Definitions /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BUTTON_PIN          0
#define VOLT_MEAS_PIN       A0
#define SEND_MSG_PIN        16 //Pin labeled "D0" is GPIO16
#define PACK_VOLTS_MAX      33600 // mvolts
#define PACK_VOLTS_MIN      21600 // mvolts
#define PACK_DISCONNECTED   5 // volts
#define PACK_VOLTS_ADC_MAX  967
#define PACK_VOLTS_ADC_MIN  629 
#define PACK_NAME           "Atlas Pack _Main_" // Choices: "Valkyrie Pack _Dewey_" ; "Valkyrie Pack _TBD_" ; "Atlas Pack _Main_" ; "Atlas Pack _Spare_" ; "Gryphon Pack _Main_" 
#define PACK_LOGO           ":atlas_logo_short: " // Choices: ":dew: " for Valkyrie Pack Dewey; TBD for Valkyrie Pack 2; ":atlas_logo_short: " for Atlas Pack Main; ":shimble: " for Atlas Pack Spare; ":gryphon_logo3: " for Gryphon Pack Main
int num_loops               = 0; //num times program has looped
int adc_reading             = 0;
float measured_voltage      = 0;
uint32_t send_msg_timer     = millis();
const uint32_t SEND_MSG     = 1800000; //30min = 1800000msec 
uint32_t num_disconnected   = 0;


#define STASSID             "MST-PSK-N"
#define STAPSK              "JoeMiner"
const char *ssid            = STASSID;
const char *pass            = STAPSK;
const char *host            = "hooks.slack.com";
const uint16_t port         = 443;
const char *path            = "/services/T5R4XNGC8/BG2THPXDG/MsG8UnB8U7tCNEyyn1D0uRb5";
std::string message         = "Hello, World!"; //the actual message to be sent, as a c_str.
int mesSize                 = 0; // later size should be set to message.size() + 11;
std::string mesSizeString   = "";
const char *messageC        = "";
const char *mesSizeC        = "";
const char *request         = "";
WiFiClientSecure client;

namespace patch
{
    template <typename T> 
    std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str();
    }
}

// Functions //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() 
{
  Serial.begin(115200);

  pinMode(SEND_MSG_PIN, OUTPUT); 
  digitalWrite(SEND_MSG_PIN, LOW); //LED is active low
  

  WiFi.mode(WIFI_STA); // We start by connecting to a WiFi network
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.begin(ssid, pass);

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
  if ((millis() - send_msg_timer > SEND_MSG) || num_loops == 0)
  {
    adc_reading = analogRead(VOLT_MEAS_PIN);
    Serial.print("ADC: ");
    Serial.println(adc_reading);

    if(adc_reading > PACK_VOLTS_ADC_MIN)
    {
      if(adc_reading > PACK_VOLTS_ADC_MAX)
      {
        adc_reading = PACK_VOLTS_ADC_MAX;
      }

      measured_voltage = map(adc_reading, PACK_VOLTS_ADC_MIN, PACK_VOLTS_ADC_MAX, PACK_VOLTS_MIN, PACK_VOLTS_MAX);
    }

    if(adc_reading < PACK_VOLTS_ADC_MIN)
    {
      measured_voltage = map(adc_reading, 0, PACK_VOLTS_ADC_MIN, 0, PACK_VOLTS_MIN);
    }
    
    measured_voltage = measured_voltage / 1000;
    measured_voltage = roundf(measured_voltage * 100) / 100; //Rounds voltage to two num after decimal
    //Serial.println(measured_voltage);

    if(measured_voltage < PACK_DISCONNECTED && num_loops != 0)
    {
      message = ":boi_matt2: ";
      message += patch::to_string(PACK_NAME);
      message += " *DISCONNECTED*";
      fetchInsecure();
      num_disconnected ++;
      send_msg_timer = millis();
      digitalWrite(SEND_MSG_PIN, HIGH); // Blink blue led to indicate msg sent
      delay(250);
      digitalWrite(SEND_MSG_PIN, LOW);
    }
    if(measured_voltage > PACK_DISCONNECTED && measured_voltage < 21.6)
    {
      Serial.print("Voltage: ");
      Serial.println(measured_voltage);
      message = "<!channel> :fire: ";
      message += patch::to_string(PACK_NAME);
      message += " DAMAGED:  *";
      message += patch::to_string(measured_voltage);
      message += "V*";
      fetchInsecure();
      send_msg_timer = millis();
      digitalWrite(SEND_MSG_PIN, HIGH); // Blink blue led to indicate msg sent
      delay(250);
      digitalWrite(SEND_MSG_PIN, LOW);
    }
    if((measured_voltage <= 23) && (measured_voltage >= 21.6))
    {
      Serial.print("Voltage: ");
      Serial.println(measured_voltage);
      message = "<!channel> :rotating_light: ";
      message += patch::to_string(PACK_NAME);
      message += " CRITICAL:  *";
      message += patch::to_string(measured_voltage);
      message += "V*";
      fetchInsecure();
      send_msg_timer = millis();
      digitalWrite(SEND_MSG_PIN, HIGH); // Blink blue led to indicate msg sent
      delay(250);
      digitalWrite(SEND_MSG_PIN, LOW);
    }
    if((measured_voltage <= 25) && (measured_voltage > 23))
    {
      Serial.print("Voltage: ");
      Serial.println(measured_voltage);
      message = ":warning: ";
      message += patch::to_string(PACK_NAME);
      message += "LOW:  *";
      message += patch::to_string(measured_voltage);
      message += "V*";
      fetchInsecure();
      send_msg_timer = millis();
      digitalWrite(SEND_MSG_PIN, HIGH); // Blink blue led to indicate msg sent
      delay(250);
      digitalWrite(SEND_MSG_PIN, LOW);
    }
    if(measured_voltage > 25)
    {
      Serial.print("Voltage: ");
      Serial.println(measured_voltage);
      message = patch::to_string(PACK_LOGO);
      message += patch::to_string(PACK_NAME);
      message += ":  *";
      message += patch::to_string(measured_voltage);
      message += "V*";
      fetchInsecure();
      send_msg_timer = millis();
      digitalWrite(SEND_MSG_PIN, HIGH); // Blink blue led to indicate msg sent
      delay(250);
      digitalWrite(SEND_MSG_PIN, LOW);
    }   
  }
  num_loops ++;
}

// Try and connect using a WiFiClientBearSSL to specified host:port and dump HTTP response
void fetchURL(BearSSL::WiFiClientSecure *client, const char *host, const uint16_t port, const char *path) 
{ 
  client->connect(host, port);
  if (!client->connected()) {
    return;
  }
  mesSize = message.length() + 11;
  mesSizeString = patch::to_string(mesSize);
  messageC = message.c_str();
  mesSizeC = mesSizeString.c_str();
  client->write("POST ");
  client->write(path);
  client->write(" HTTP/1.0\r\n");
  client->write("Host: ");
  client->write(host);
  client->write("\r\nUser-Agent: Arduino/1.1\r\n");
  client->write("Accept: */*\r\nContent-type: application/json\r\nContent-Length: ");
  client->write(mesSizeC);
  client->write("\r\n\r\n{\"text\":\"");
  client->write(messageC);
  client->write("\"}");
}

void fetchInsecure() 
{
  BearSSL::WiFiClientSecure client;
  client.setInsecure();
  fetchURL(&client, host, port, path);
}