 #include <ESP8266WiFi.h>
 #include <string>
 #include <sstream>
//finally found a good source https://forum.arduino.cc/index.php?topic=155218.0
const char BUTTON_PIN = 0;
#define STASSID "MST-PSK-N"
#define STAPSK "JoeMiner"
#define HOST "hooks.slack.com"
#define URL "/services/T5R4XNGC8/BG2THPXDG/MsG8UnB8U7tCNEyyn1D0uRb5"
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
  if (!path) {
    path = "/";
  }

  ESP.resetFreeContStack();
  uint32_t freeStackStart = ESP.getFreeContStack();
  Serial.printf("Trying: %s:443...", host);
  client->connect(host, port);
  if (!client->connected()) {
    Serial.printf("*** Can't connect. ***\n-------\n");
    return;
  }
  Serial.printf("Connected!\n-------\n");
  message = "Can it be really really big?"; //
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


  /*client.println("POST /tinyFittings/index.php HTTP/1.1");
  client.println("Host:  artiswrong.com");
  client.println("User-Agent: Arduino/1.0");
  client.println("Connection: close");
  client.println("Content-Type: application/x-www-form-urlencoded;");
  client.print("Content-Length: ");
  client.println(PostData.length());
  client.println();
  client.println(PostData);*/
  uint32_t to = millis() + 5000;
  if (client->connected()) {
    do {
      char tmp[64];
      memset(tmp, 0, 64);
      int rlen = client->read((uint8_t*)tmp, sizeof(tmp) - 1);
      yield();
      if (rlen < 0) {
        break;
      }
      // Only print out first line up to \r, then abort connection
      char *nl = strchr(tmp, '\r');
      if (nl) {
        *nl = 0;
        Serial.print(tmp);
        break;
      }
      Serial.print(tmp);
    } while (millis() < to);
  }
  client->stop();
  uint32_t freeStackEnd = ESP.getFreeContStack();
  Serial.printf("\nCONT stack used: %d\n-------\n\n", freeStackStart - freeStackEnd);
}



void fetchInsecure() {
  Serial.printf(R"EOF(
This is absolutely *insecure*, but you can tell BearSSL not to check the
certificate of the server.  In this mode it will accept ANY certificate,
which is subject to man-in-the-middle (MITM) attacks.
)EOF");
  BearSSL::WiFiClientSecure client;
  client.setInsecure();
  fetchURL(&client, host, port, path);
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  fetchInsecure();
}


uint32_t timer = millis();
void loop() {
  
}
