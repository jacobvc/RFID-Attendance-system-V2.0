#include "ESP32_RFID.h"
#include "RfidSound.h"
#include "RfidLcd.h"

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

void SetTz(String value)
{
  if (GetTzValue(value).length() > 0) {
    configTzTime(GetTzValue(value).c_str(), "pool.ntp.org", "time.nist.gov");
  }
}
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
  SetTz(tz);
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
        LcdDisplayNotifyArrive(payload.substring(5));
        RfidSoundArrive();
      } else if (payload.substring(0, 6) == "logout") {
        LcdDisplayNotifyDepart(payload.substring(6));
        RfidSoundDepart();
      } else if (payload == "succesful") {
        LcdDisplayNotifySuccessful(payload);
      } else if (payload == "available") {
        LcdDisplayNotifyAvailable(payload);
      } else {
        LcdDisplayNotifyError(payload);
      }
      delay(100);
      http.end();  //Close connection
    }
  }
}
char scanList[200];
void connectToWiFi(const char *ssid, const char *pw) {
  int scanCount = WiFi.scanNetworks();
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
    LcdDisplayConnecting(ssid);

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
    LcdDisplayApMode(CONFIG_AP_SSID, WiFi.softAPIP());

    Serial.println("Access point: " CONFIG_AP_SSID);
    Serial.println(WiFi.softAPIP());
    apmode = true;
  } 
  else {
    apmode = false;
    Serial.println("");
    Serial.println("Connected");

    LcdDisplayConnected(WiFi.localIP());

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
    String tmpTz = server.arg("tz");

    Serial.println(" ssid: " + server.arg("ssid"));
    Serial.println(" pw: " + server.arg("pw"));
    Serial.println(" device_token: " + server.arg("device_token"));
    Serial.println(" url: " + server.arg("url"));
    Serial.println(" tz: " + server.arg("tz"));
    Serial.println(" tz name: " + GetTzName(tmpTz));
    Serial.println(" tz value: " + GetTzValue(tmpTz));
    if (GetTzValue(tmpTz).length() > 0) {
      SetTz(tmpTz);
      tz = tmpTz;
      preferences.putString("tz", tz);
   }

    preferences.putString("device_token", device_token);
    preferences.putString("url", url);

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
  <script>
  </script>
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
    #tz,input[type=text] {
      width: 300px;
    }
  </style>
  <script>
    function ssidInit()
    {
      var theList = document.getElementById('ssidList');  
      var theinput = document.getElementById('ssid');  

      var optList=%s;
      optList.forEach(function (name) {
        var option = document.createElement("option");
        option.text = name;
        option.value = name;
        theList.add(option);
      });
    }

    function combo(theList, theinput)
    {
      theinput = document.getElementById(theinput);  
      var idx = theList.selectedIndex;
      var content = theList.options[idx].innerHTML;
      theinput.value = content;	
    }
  </script>
</head>
<body onload="addtimezone('tz'); ssidInit() "></body>
  <h1>RFID Scanner Configuration</h1>
  <form method="POST">
    <label for="ssid">SSID</label>
    <input type="text" id="ssid" name="ssid" value="%s">
      <select id="ssidList" onChange="combo(this, 'ssid');"></select>
<br>
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
)",scanList,  ssid.c_str(), tzname, device_token.c_str(), url.c_str());
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

