#include "RfidWiFiServer.h"

#include "incbin.h" // Use the incbin library to embed files

// Embed the web files
INCTXT(Timezone_js, "html/timezone.js");
INCTXT(ChangingAp, "html/changingap.html");
INCTXT(Index, "html/index.html");
INCTXT(Connecting, "html/connecting.html");

/*
 * Variables
 *
 */
RfidWifiServer *RfidWifiServer::exec;


char temp[4000];

/*
 *
 * Static handlers
 *
 */

static void handleRoot() {
  RfidWifiServer::exec->root();
}
static void handleNotFound() {
  RfidWifiServer::exec->NotFound();
}

static void handleTimezoneJs() {
  RfidWifiServer::exec->TimezoneJs();
}


/*
 *
 * Implementation
 *
 */
void RfidWifiServer::connectToWiFi(const char *ssid, const char *pw) {
  WifiServer::connectToWiFi(ssid, pw);
  server->on("/", handleRoot);
  server->on("/timezones.js", handleTimezoneJs);
  server->onNotFound(handleNotFound);

}

#ifdef ADJUSTABLE_BACKLIGHT
#define BACKLIGHT_VISIBILITY ""
#else
#define BACKLIGHT_VISIBILITY "class='hidden'"
#endif

void RfidWifiServer::root() 
{
  if (server->args() > 0) {
    String newSsid;
    if (server->arg("backlight") != "") {
      Serial.println("Backlight = " + server->arg("backlight"));
      int val = atoi(server->arg("backlight").c_str());
      if (val >= 50 && val < 256) {
        backlight = val;
        preferences.putInt("backlight", val);
      }
    }
    else {
      url = server->arg("url");
      newSsid = server->arg("ssid");
      password = server->arg("pw");
      device_token = server->arg("device_token");
      String tmpTz = server->arg("tz");

      Serial.println(" ssid: " + server->arg("ssid"));
      Serial.println(" pw: " + server->arg("pw"));
      Serial.println(" device_token: " + server->arg("device_token"));
      Serial.println(" url: " + server->arg("url"));
      Serial.println(" tz: " + server->arg("tz"));
      Serial.println(" tz name: " + GetTzName(tmpTz));
      Serial.println(" tz value: " + GetTzValue(tmpTz));
      if (GetTzValue(tmpTz).length() > 0) {
        SetTz(tmpTz);
        tz = tmpTz;
        preferences.putString("tz", tz);
      }

      preferences.putString("device_token", device_token);
      preferences.putString("url", url);
    }

    if (apmode) {
      // Check to try connect
      // Id there is a new ssid, try to use it
      if (!newSsid.isEmpty()) {
        ssid = newSsid;
        preferences.putString("ssid", ssid);
        preferences.putString("password", password);
        Serial.println("Trying access point " + ssid);
        snprintf(temp, sizeof(temp),  gConnectingData, ssid.c_str());
        server->send(200, "text/html", temp);
        delay(100);

        MDNS.end();
        server->close();
        WiFi.softAPdisconnect();
        WiFi.disconnect();
        apmode = false;
        delay(100);
        return;
      }
    }
    else {
      if (!newSsid.isEmpty() && !ssid.equals(newSsid)) {
        Serial.println("Trying access point " + newSsid);
        snprintf(temp, sizeof(temp), gChangingApData, ssid.c_str(), newSsid.c_str());
        server->send(200, "text/html", temp);
        ssid = newSsid;
        preferences.putString("ssid", ssid);
        preferences.putString("password", password);
        delay(100);

        MDNS.end();
        server->close();
        WiFi.disconnect();
        delay(100);
        return;
      }
    }
  } 
  else {
    Serial.println("root url: NO ARGS");
  }

  digitalWrite(led, 1);

  char tmpTz[80];
  strcpy(tmpTz, tz.c_str());
  const char *tzname = strtok(tmpTz, ";");
  if (!tzname) tzname = "";

  snprintf(temp, sizeof(temp), gIndexData,scanList, BACKLIGHT_VISIBILITY, 
    backlight, ssid.c_str(), tzname, device_token.c_str(), url.c_str());
  server->send(200, "text/html", temp);
  digitalWrite(led, 0);
}

void RfidWifiServer::NotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += (server->method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";

  for (uint8_t i = 0; i < server->args(); i++) {
    message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
  }

  server->send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void RfidWifiServer::TimezoneJs() {
  extern const char *timezone_js;
  server->send(200, "text/plain", gTimezone_jsData);
}

