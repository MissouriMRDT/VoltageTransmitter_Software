#define STASSID             "MST-PSK-N"
#define STAPSK              "JoeMiner"
const char *ssid            = STASSID;
const char *pass            = STAPSK;
const char *host            = "hooks.slack.com";
const uint16_t port         = 443;
const char *path            = "/services/T5R4XNGC8/BG2THPXDG/MsG8UnB8U7tCNEyyn1D0uRb5";
const char *test1 			= "21";
const char *test2			= "test1test1";

void fetchURL(BearSSL::WiFiClientSecure *client, const char *host, const uint16_t port, const char *path, const char *messageC, const char *mesSizeC) 
{ 
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
  client->write(test1);
  client->write("\r\n\r\n{\"text\":\"");
  client->write(test2);
  client->write("\"}");
}

void fetchInsecure(BearSSL::WiFiClientSecure *client, const char* messageC, const char* mesSizeC) 
{
  client->setInsecure();
  fetchURL(client, host, port, path, messageC, mesSizeC);
}