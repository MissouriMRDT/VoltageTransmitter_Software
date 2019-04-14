#define STASSID             "MST-PSK-N"
#define STAPSK              "JoeMiner"
const char *ssid            = STASSID;
const char *pass            = STAPSK;
const char *host            = "hooks.slack.com";
const uint16_t port         = 443;
const char *path            = "/services/T5R4XNGC8/BG2THPXDG/MsG8UnB8U7tCNEyyn1D0uRb5";
int mesSize = 0;
std::string mesSizeString = "";
const char *messageC = "";
const char *mesSizeC = "";

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

void postURL(BearSSL::WiFiClientSecure *client, const char *host, const uint16_t port, const char *path, std::string message) 
{ 
  
  mesSize = message.length() + 11;
  //mesSizeString = patch::to_string(mesSize);
  messageC = message.c_str();
  mesSizeC = (patch::to_string(mesSize)).c_str();
  client->connect(host, port);
  if (!client->connected()) {
    return;
  }
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

void postInsecure(BearSSL::WiFiClientSecure *client, std::string message) 
{
  client->setInsecure();
  postURL(client, host, port, path, message);
}