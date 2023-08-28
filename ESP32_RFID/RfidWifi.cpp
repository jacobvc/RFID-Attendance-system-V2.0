#include "ESP32_RFID.h"
#include "RfidSound.h"
//************************* Biometric Icons *********************************
#define Wifi_start_width 54
#define Wifi_start_height 49
const uint8_t PROGMEM Wifi_start_bits[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x07, 0xc0, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x38, 0x00, 0x07, 0xc0, 0x00, 0x38, 0x00, 0x70, 0x00, 0xff, 0xfe, 0x00, 0x1e, 0x00, 0xe0, 0x03, 0xfc, 0x7f, 0xc0, 0x0e, 0x00, 0x00, 0x1f, 0x80, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x01, 0xe0, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x07, 0x1e, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x1f, 0xbf, 0x80, 0x00, 0x03, 0xe0, 0x04, 0x7f, 0xff, 0xc0, 0x00, 0x07, 0x80, 0x00, 0xff, 0xff, 0xe0, 0x00, 0x0e, 0x00, 0x00, 0xff, 0xff, 0xe0, 0x00, 0x0c, 0x00, 0x00, 0x7f, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x03, 0xf8, 0x03, 0xf8, 0x00, 0x00, 0x07, 0xe7, 0xf9, 0xf1, 0xfc, 0x00, 0x00, 0x1f, 0xe7, 0xf1, 0xf9, 0xfc, 0x00, 0x00, 0x1f, 0xe7, 0xf3, 0xf9, 0xfc, 0x00, 0x00, 0x3f, 0xe7, 0xf3, 0xf9, 0xfc, 0x00, 0x00, 0x3f, 0xe7, 0xf1, 0xf1, 0xfc, 0x00, 0x00, 0x3f, 0xe3, 0xf8, 0xe3, 0xfc, 0x00, 0x00, 0x3f, 0xf3, 0xfc, 0x07, 0xf8, 0x00, 0x00, 0x1f, 0xf0, 0x7f, 0x0f, 0xc0, 0x00, 0x00, 0x0f, 0xe0, 0x7f, 0xff, 0xe0, 0x00, 0x00, 0x07, 0xc0, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xbf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#define Wifi_connected_width 63
#define Wifi_connected_height 49
const uint8_t PROGMEM Wifi_connected_bits[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x3f, 0xff, 0xc0, 0x07, 0xff, 0xf8, 0x00, 0x00, 0xff, 0xf8, 0x00, 0x00, 0x3f, 0xfe, 0x00, 0x03, 0xff, 0x80, 0x00, 0x00, 0x03, 0xff, 0x80, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0xff, 0xc0, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x3f, 0xe0, 0x01, 0xff, 0xff, 0x00, 0x0f, 0xf8, 0x7f, 0x80, 0x0f, 0xff, 0xff, 0xe0, 0x03, 0xfc, 0xff, 0x00, 0x7f, 0xff, 0xff, 0xfc, 0x01, 0xfe, 0xfc, 0x01, 0xff, 0xff, 0xff, 0xff, 0x00, 0x7e, 0x78, 0x07, 0xff, 0xc0, 0x07, 0xff, 0xc0, 0x3c, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x07, 0xf8, 0x00, 0x00, 0x7f, 0x00, 0x01, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x7e, 0x00, 0x7f, 0xfc, 0x00, 0xfc, 0x00, 0x00, 0x3c, 0x03, 0xff, 0xff, 0x80, 0x78, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x80, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/*
 *
 * Variables
 *
 */
String ssid;
String password;
String device_token;
String url;
String tz;

const int led = 2;
bool apmode = false;
char temp[4000];

Preferences preferences;
WebServer server(80);

/*
 *
 * Static prototypes
 *
 */
static void handleNotFound();
static void handleRoot();
static void handleTimezoneJs();

/*
 *
 * Implementation
 *
 */

void RfidWiFiSetup(void) {
  preferences.begin("rfid-scanner", false);
  ssid = preferences.getString("ssid", "");
  password = preferences.getString("password", "");
  device_token = preferences.getString("device_token", "");
  url = preferences.getString("url", "");
  tz = preferences.getString("tz", "");

  pinMode(led, OUTPUT);
  digitalWrite(led, 0);

  connectToWiFi(ssid.c_str(), password.c_str());
}

void RfidWiFiTick(void) {
  //check if there's a connection to Wi-Fi or not
  if (!WiFi.isConnected() && !apmode) {
    connectToWiFi(ssid.c_str(), password.c_str());  //Retry to connect to Wi-Fi
  } else {
    server.handleClient();
  }
}

bool RfidWiFiApMode() {
  return apmode;
}

//************send the Card UID to the website*************
void SendCardID(String Card_uid) {
  Serial.println("Sending the Card ID");
  if (WiFi.isConnected()) {
    String getData, Link;
    HTTPClient http;  //Declare object of class HTTPClient
    //GET Data
    getData = "?card_uid=" + String(Card_uid) + "&device_token=" + String(device_token);  // Add the Card ID to the GET array in order to send it
    //GET methode
    Link = url + getData;
    Serial.println(Link);
    http.begin(Link);  //initiate HTTP request   //Specify content-type header

    int httpCode = http.GET();          //Send the request
    String payload = http.getString();  //Get the response payload

    //    Serial.println(Link);   //Print HTTP return code
    Serial.println(httpCode);  //Print HTTP return code
    Serial.println(Card_uid);  //Print Card ID
    Serial.println(payload);   //Print request response payload

    if (httpCode == 200) {
      if (payload.substring(0, 5) == "login") {
        String user_name = payload.substring(5);
        //  Serial.println(user_name);

        CLEAR_DISPLAY;
        display.setTextSize(2);            // Normal 2:2 pixel scale
        display.setTextColor(TEXT_COLOR);  // Draw white text
        display.setCursor(15, 0);          // Start at top-left corner
        display.print(F("Welcome"));
        display.setCursor(0, 20);
        display.print(user_name);
        UPDATE_DISPLAY;
        sound_arrive();
      } else if (payload.substring(0, 6) == "logout") {
        String user_name = payload.substring(6);
        //  Serial.println(user_name);

        CLEAR_DISPLAY;
        display.setTextSize(2);            // Normal 2:2 pixel scale
        display.setTextColor(TEXT_COLOR);  // Draw white text
        display.setCursor(10, 0);          // Start at top-left corner
        display.print(F("Good Bye"));
        display.setCursor(0, 20);
        display.print(user_name);
        UPDATE_DISPLAY;
        sound_depart();
      } else if (payload == "succesful") {
        CLEAR_DISPLAY;
        display.setTextSize(2);            // Normal 2:2 pixel scale
        display.setTextColor(TEXT_COLOR);  // Draw white text
        display.setCursor(5, 0);           // Start at top-left corner
        display.print(F("New Card"));
        UPDATE_DISPLAY;
      } else if (payload == "available") {
        CLEAR_DISPLAY;
        display.setTextSize(2);            // Normal 2:2 pixel scale
        display.setTextColor(TEXT_COLOR);  // Draw white text
        display.setCursor(5, 0);           // Start at top-left corner
        display.print(F("Free Card"));
        UPDATE_DISPLAY;
      } else {
        CLEAR_DISPLAY;
        display.setTextSize(2);            // Normal 2:2 pixel scale
        display.setTextColor(ST77XX_RED);  // Draw white text
        display.setCursor(5, 0);           // Start at top-left corner
        display.print(payload);
        UPDATE_DISPLAY;
      }
      delay(100);
      http.end();  //Close connection
    }
  }
}
void connectToWiFi(const char *ssid, const char *pw) {
  if (*ssid) {
    // Only try to connect if ssid is not blank
    WiFi.mode(WIFI_OFF);  //Prevents reconnection issue (taking too long to connect)
    delay(1000);
    WiFi.mode(WIFI_STA);
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, pw);

    CLEAR_DISPLAY;
    display.setTextSize(2);            // Normal 1:1 pixel scale
    display.setTextColor(TEXT_COLOR);  // Draw white text
    display.setCursor(0, 0);           // Start at top-left corner
    display.print(F("Connecting to \n"));
    display.setCursor(0, 60);
    display.setTextSize(2);
    display.print(ssid);
    display.drawBitmap((SCREEN_WIDTH - Wifi_start_width) / 2, 10, Wifi_start_bits, Wifi_start_width, Wifi_start_height, TEXT_COLOR);
    UPDATE_DISPLAY;
    int retries = 30;
    while (WiFi.status() != WL_CONNECTED && retries-- > 0) {
      delay(500);
      Serial.print(".");
    }
  }
  if (WiFi.status() != WL_CONNECTED) {
    if (!WiFi.softAP(CONFIG_AP_SSID, "")) {
      Serial.println("Soft AP creation failed.");
      while (1)
        ;
    }
    CLEAR_DISPLAY;
    display.setTextSize(2);
    display.setTextColor(RED);  // Draw white text
    display.setCursor(8, 3);           // Start at top-left corner
    display.print(F("NO WiFi\n"));
    display.setTextColor(TEXT_COLOR);  // Draw white text
    display.print(F("\nConnect to AP\n "));
    display.setTextColor(BLUE);  // Draw white text
    display.print(CONFIG_AP_SSID);
    display.setTextColor(TEXT_COLOR);  // Draw white text
    display.print(F("\nAnd Browse to\n "));
    display.setTextColor(BLUE);  // Draw white text
    display.print(WiFi.softAPIP());
    UPDATE_DISPLAY;

    Serial.println("Access point: " CONFIG_AP_SSID);
    Serial.println(WiFi.softAPIP());
    apmode = true;
  } 
  else {
    apmode = false;
    Serial.println("");
    Serial.println("Connected");

    CLEAR_DISPLAY;
    display.setTextSize(1);            // Normal 1:1 pixel scale
    display.setTextColor(TEXT_COLOR);  // Draw white text
    display.setCursor(0, 0);           // Start at top-left corner
    display.print(F("Connected "));
    display.print(WiFi.localIP());
    display.drawBitmap((SCREEN_WIDTH - Wifi_connected_width) / 2, 15, Wifi_connected_bits, Wifi_connected_width, Wifi_connected_height, TEXT_COLOR);
    UPDATE_DISPLAY;

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  }
  // If we got here, wifi is connected
  // Start MDNS and the http server
  if (MDNS.begin("RFID Scanner")) {
    Serial.println("MDNS responder started");
  }
  server.begin();
  MDNS.addService("http", "tcp", 80);

  server.on("/", handleRoot);
  server.on("/timezones.js", handleTimezoneJs);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  delay(3000);
}
//=======================================================================

static void handleRoot() {
  if (server.args() > 0) {
    url = server.arg("url");
    String newSsid = server.arg("ssid");
    password = server.arg("pw");
    device_token = server.arg("device_token");
    tz = server.arg("tz");

    Serial.println(" ssid: " + server.arg("ssid"));
    Serial.println(" pw: " + server.arg("pw"));
    Serial.println(" device_token: " + server.arg("device_token"));
    Serial.println(" url: " + server.arg("url"));
    Serial.println(" tz: " + server.arg("tz"));

    preferences.putString("device_token", device_token);
    preferences.putString("url", url);
    preferences.putString("tz", tz);

    if (apmode) {
      // Check to try connect
      // Id there is a new ssid, try to use it
      if (!newSsid.isEmpty()) {
        ssid = newSsid;
        preferences.putString("ssid", ssid);
        preferences.putString("password", password);
        Serial.println("Trying access point " + ssid);
        snprintf(temp, sizeof(temp),  R"(
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Scanner Configuration</title>
  <style>
    body {
      background-color: #cccccc;
      font-family: Arial, Helvetica, Sans-Serif;
      Color: #000088;
    }
  </style>
</head>
<body>
  <h1>RFID Scanner Configuration</h1><br>
  <h2>Connecting to access point %s</h2>
</body>
</html>
        )", ssid.c_str());
        server.send(200, "text/html", temp);
        delay(100);

        MDNS.end();
        server.close();
        WiFi.softAPdisconnect();
        WiFi.disconnect();
        apmode = false;
        delay(100);
        return;
      }
    }
    else {
      if (!ssid.equals(newSsid)) {
        Serial.println("Trying access point " + newSsid);
        snprintf(temp, sizeof(temp),  R"(
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Scanner Configuration</title>
  <style>
    body {
      background-color: #cccccc;
      font-family: Arial, Helvetica, Sans-Serif;
      Color: #000088;
    }
  </style>
</head>
<body>
  <h1>RFID Scanner Configuration</h1><br>
  <h2>Changing access point from %s to %s</h2>
</body>
</html>
        )", ssid.c_str(), newSsid.c_str());
        server.send(200, "text/html", temp);
        ssid = newSsid;
        preferences.putString("ssid", ssid);
        preferences.putString("password", password);
        delay(100);

        MDNS.end();
        server.close();
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

  snprintf(temp, sizeof(temp), R"(
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <script type="application/javascript" src="timezones.js"></script>
  <title>Scanner Configuration</title>
  <style>
    body {
      background-color: #cccccc;
      font-family: Arial, Helvetica, Sans-Serif;
      Color: #000088;
    }
    label {
      display: inline-block;
      width: 110px;
    }
    select, input[type=text] {
      width: 250px;
    }
  </style>
  <script>
  </script>
</head>
<body onload="addtimezone('tz') "></body>
  <h1>RFID Scanner Configuration</h1>
  <form method="POST">
    <label for="ssid">SSID</label>
    <input type="text" id="ssid" name="ssid" value="%s"><br>
    <label for="pw">Password</label>
    <input type="text" id="pw" name="pw" value=""><br>
    <label for="tz">Timezone</label>
    <select type="text" id="tz" name="tz">
      <option selected="selected">%s</option>
    </select><br>
    <label for="device_token">Device Token</label>
    <input type="text" id="device_token" name="device_token" value="%s"><br>
    <label for="url">Server URL</label>
    <input type="text" id="url" name="url" value="%s"><br><br>
    <input type="submit" value="Submit">
  </form>
</body>
</html>
)", ssid.c_str(), tzname, device_token.c_str(), url.c_str());
  server.send(200, "text/html", temp);
  digitalWrite(led, 0);
}

static void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}
static void handleTimezoneJs() {
  extern const char *timezone_js;
  server.send(200, "text/plain", timezone_js);
}

