// my-config.h
#if __has_include("my-config.h")
#include "my-config.h"
#else
#define POST_SERVER "test.org"
#define POST_PORT 80
#define POST_PATH "/"
#endif

// AK030
#include <AK-030.h>

#define APN "soracom.io"
#define USER "sora"
#define PASSWD "sora"
#define PPPAUTH "PAP"

AK030 *ak030 = new AK030();

#include "lib-ak030.h"

// Post Body
#include <ArduinoJson.h>
StaticJsonDocument<128> doc;

void setup() {
  Serial.begin(115200);
  ak030->begin(APN, USER, PASSWD, PPPAUTH);
  delay(1000);
}

void loop() {

  // sample post data
  doc["device"] = "DUMMY_TEST2";
  doc["temp"] = "24.5";
  doc["humd"] = "60.5";

  // post body
  char body[128];
  serializeJson(doc, body);
  Serial.println(body);

  const char req_template[] =
      "POST %s HTTP/1.1\r\n"
      "Host: %s\r\n"
      "Accept: application/json\r\n"
      "Content-Length: %d\r\n"
      "\r\n"
      "%s";

  int bodySize = strlen(body);

  static char req[1024];
  snprintf(req, sizeof(req), req_template, POST_PATH, POST_SERVER, bodySize, body);


  // AK030 Process
  LibAK030::Connect(ak030);
  auto ip = LibAK030::DnsLookup(ak030, POST_SERVER);
  Serial.println(ip);
  if (ip == nullptr) {
    Serial.println("[ERROR] DNS lookup Fail");
    return;
  }
  // TCP Open
  auto tcpStatus = LibAK030::OpenTCP(ak030, ip, POST_PORT);
  Serial.println(tcpStatus);
  if (tcpStatus == false) {
    Serial.println("[ERROR] TCP Open Fail");
    return;
  }
  // Request 
  auto reqStatus = LibAK030::RequestPost(ak030, req);
  Serial.println(reqStatus);

  delay(100000);
}
