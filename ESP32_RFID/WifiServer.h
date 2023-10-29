#pragma once

#include <Preferences.h>
#include <time.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include "Notify.h"

/** Support for WiFi and WiFi hosted web server  
 *
 * Attempts to connect to AP, and if not, restarts in AP Mode and
 * supports serving a configuration web page 
 */
class WifiServer {
public:
  int led;
  bool apmode;
  Preferences& preferences;
  String mdnsName;

  String ssid;
  String password;
  String tz;
  WebServer *server;
  Notify *notify;

  WifiServer(Preferences& preferences, Notify *notify, String mdnsName) 
    : preferences(preferences), notify(notify) {
    this->mdnsName = mdnsName;
    led = 2;
    apmode = false;
    server = new WebServer(80);
  }

  void begin(void) {
    ssid = preferences.getString("ssid", "");
    password = preferences.getString("password", "");
    tz = preferences.getString("tz", "");

    pinMode(led, OUTPUT);
    digitalWrite(led, 0);

    connectToWiFi(ssid.c_str(), password.c_str());
    SetTz(tz);
     delay(3000);
  }
  
  void Tick(void) {
    //check if there's a connection to Wi-Fi or not
    if (!WiFi.isConnected() && !apmode) {
      connectToWiFi(ssid.c_str(), password.c_str());  //Retry to connect to Wi-Fi
       delay(3000);
    } else {
      server->handleClient();
    }
  }

  char scanList[200];
virtual void connectToWiFi(const char *ssid, const char *pw) {
  int scanCount = 0;
  scanCount = WiFi.scanNetworks();
  strcpy(scanList, "[");
  for (int i = 0; i < scanCount; ++i) {
    if (strlen( WiFi.SSID(i).c_str()) + strlen(scanList) < sizeof(scanList) - 1) {
      strcat(scanList, "\"");
      strcat(scanList,  WiFi.SSID(i).c_str());
      strcat(scanList, "\",");
    }
  }
  scanList[strlen(scanList) - 1] = ']';
  Serial.println(scanList);
  WiFi.scanDelete();

  if (*ssid) {
    // Only try to connect if ssid is not blank
    WiFi.mode(WIFI_OFF);  //Prevents reconnection issue (taking too long to connect)
    delay(1000);
    WiFi.mode(WIFI_STA);
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pw);
    notify->Connecting(ssid, true);

    int retries = 30;
    while (WiFi.status() != WL_CONNECTED && retries-- > 0) {
      delay(500);
      notify->Connecting(ssid, retries & 1);
      Serial.print(".");
    }
  }
  if (WiFi.status() != WL_CONNECTED) {
    if (!WiFi.softAP(CONFIG_AP_SSID, "")) {
      Serial.println("Soft AP creation failed.");
      while (1)
        ;
    }
    notify->ApMode(CONFIG_AP_SSID, WiFi.softAPIP());

    Serial.println("Access point: " CONFIG_AP_SSID);
    Serial.println(WiFi.softAPIP());
    apmode = true;
  } 
  else {
    apmode = false;
    Serial.println("");
    Serial.println("Connected");

    notify->Connected(WiFi.localIP());

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  }
  // If we got here, wifi is connected
  // Start MDNS and the http server
  if (MDNS.begin(mdnsName.c_str())) {
    Serial.println("MDNS responder started");
  }
  server->begin();
  MDNS.addService("http", "tcp", 80);
  Serial.println("HTTP server started");
 }

  String get(String getData)
  {
  String payload = "WiFi Not connected";
  if (WiFi.isConnected()) {
    String Link;
    HTTPClient http;  //Declare object of class HTTPClient
    //GET methode
    Link = url + getData;
    Serial.println("GET Request: " + Link);
    http.begin(Link);  //initiate HTTP request 

    int httpCode = http.GET();          //Send the request
    payload = http.getString();  //Get the response payload

    Serial.println("HTTP Response code: " + String(httpCode));  //Print HTTP return code
    Serial.println("Response Payload: " + payload);   //Print request response payload

    if (httpCode == 200) {
      delay(100);
      http.end();  //Close connection
    }
    else {
      payload = "html GET error: " + String(httpCode);
    }
  }
  return payload;
  }

  bool ApMode(void) {
    return apmode;
  }

  String GetTzName(String tz) {
    char tmpTz[80];
    strcpy(tmpTz, tz.c_str());
    const char *tzname = strtok(tmpTz, ";");
    if (!tzname) tzname = "";
    return tzname;
  }
  String GetTzValue(String tz) {
    char tmpTz[80];
    strcpy(tmpTz, tz.c_str());
    const char *tzname = strtok(tmpTz, ";");
    const char *tzvalue = strtok(NULL, ";");
    if (!tzvalue) return "";
    else return tzvalue;
  }

  void SetTz(String value) {
    if (GetTzValue(value).length() > 0) {
      configTzTime(GetTzValue(value).c_str(), "pool.ntp.org", "time.nist.gov");
    }
  }
};
