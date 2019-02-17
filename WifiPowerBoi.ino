 #include <ESP8266WiFi.h>
 #include <sstream>
//finally found a good source https://forum.arduino.cc/index.php?topic=155218.0
//https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/BearSSL_Validation/BearSSL_Validation.ino is being used for the fetchInsecure() and other things.
const char BUTTON_PIN = 0;
#define STASSID "MST-PSK-N"
#define STAPSK "JoeMiner"
const char *ssid = STASSID;
const char *pass = STAPSK;
const char *host = "hooks.slack.com";
const uint16_t port = 443;
const char *path = "/services/T5R4XNGC8/BG2THPXDG/MsG8UnB8U7tCNEyyn1D0uRb5";
std::string message = "Hello, World!"; //the actual message to be sent, as a c_str.
int mesSize = 0; // later size should be set to message.size() + 11;
std::string mesSizeString = "";
const char *messageC = "";
const char *mesSizeC = "";
  WiFiClientSecure client;
  const char *request = "";
  int pin = 0;
ADC_MODE(ADC_VCC) //set ADC pin to read mode.
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

// Try and connect using a WiFiClientBearSSL to specified host:port and dump HTTP response
void fetchURL(BearSSL::WiFiClientSecure *client, const char *host, const uint16_t port, const char *path) {
  
  client->connect(host, port);
  if (!client->connected()) {
    return;
  }
  mesSize = message.length() + 11;
  mesSizeString = patch::to_string(mesSize);
  messageC = message.c_str();
  mesSizeC = mesSizeString.c_str();
  /*client->write("GET ");
  client->write(path);
  client->write(" HTTP/1.0\r\nHost: ");
  client->write(host);
  client->write("\r\nUser-Agent: ESP8266\r\n");
  client->write("\r\n");*/
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



void fetchInsecure() {
  BearSSL::WiFiClientSecure client;
  client.setInsecure();
  fetchURL(&client, host, port, path);
}

void setup() {
  Serial.begin(115200);

  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

}


uint32_t timer = millis();
void loop() {
  
    fetchInsecure();
  if (timer > millis())
  {
    timer = millis();
  }
  if (millis() - timer > 2000)
  {
    pin = analogRead(A0);
    Serial.print(pin);
    Serial.print("\r\n");
    message = patch::to_string(pin);
    fetchInsecure();
  }
}
