// lib-ak030.cpp
#include "lib-ak030.h"

#include <Arduino.h>

static char http_response[5000];

LibAK030::LibAK030() {}

void LibAK030::Show() { Serial.println("test"); }

bool LibAK030::Connect(AK030 *ak030) {
  if (!ak030->connected()) {
    Serial.println("connecting to LTE network");
    ak030->connect();  // connect to LTE network
    if (ak030->ng()) {
      Serial.println("=> cannot connect to LTE network");
      return false;
    }
    Serial.println("=> connected");
  }
  return true;
}

bool LibAK030::Close(AK030 *ak030) {
  ak030->close();
  return true;
}

const char *LibAK030::DnsLookup(AK030 *ak030, const char *hostname) {
  const char *ipaddr = ak030->dnsLookup(hostname);
  if (ak030->ng()) {
    Serial.println("dns lookup fail");
    return nullptr;
  }
  return ipaddr;
}

bool LibAK030::OpenTCP(AK030 *ak030, const char *ip, int port) {
  Serial.printf("open tcp: ipaddr=%s port=%d\n", ip, port);
  ak030->openTcp(ip, port);
  if (!ak030->ok()) {
    Serial.println("=> cannot open tcp");
    return false;
  }
  Serial.println("=> opened");
  return true;
}

bool LibAK030::RequestPost(AK030 *ak030, const char *req) {
  Serial.println(req);

  ak030->send(req);
  if (!ak030->ok()) {
    Serial.println("=> send() failed");
    ak030->close();
    return false;
  }

  int total_size = 0;
  int left_size = sizeof(http_response);
  ak030->waitEvent(30);  // wait for socket event in 30 seconds(at most)
  while (ak030->eventDataReceived()) {
    int n;
    ak030->receive(http_response + total_size, left_size, &n);
    total_size += n;
    left_size -= n;
    if (left_size <= 0) {
      Serial.println("=> overflow !!!");
      break;
    }
    ak030->waitEvent();
  }

  if (ak030->ng()) {
    Serial.println("=> receive() failed");
    ak030->close();
    return false;
  }

  Serial.printf("received %d bytes\n", total_size);
  Serial.println(">>>> received data begin >>>>");
  Serial.println(http_response);
  Serial.println("<<<< received data end <<<<<<");

  Serial.println("close tcp");
  ak030->close();

  return true;
}
