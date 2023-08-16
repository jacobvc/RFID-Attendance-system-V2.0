/* -----------------------------------------------------------------------------
  - Project: RFID attendance system using ESP32
  - Author:  https://www.youtube.com/ElectronicsTechHaIs
  - Date:  6/03/2020
   -----------------------------------------------------------------------------
  This code was created by Electronics Tech channel for 
  the RFID attendance project with ESP32.
   ---------------------------------------------------------------------------*/
//*******************************libraries********************************
#include <Preferences.h>
#include <time.h>
//ESP32----------------------------
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
//RFID-----------------------------
#include <SPI.h>
#include <MFRC522.h>
//DISPLAY-----------------------------
#define USING_ST7735
#include <Adafruit_GFX.h>  //https://github.com/adafruit/Adafruit-GFX-Library
#ifdef USING_ST7735

// ST7735
#include <Adafruit_ST7735.h>  //https://github.com/adafruit/Adafruit_ST7735
#define TFT_CS 5
#define TFT_RST 4  // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 2

#define SCREEN_WIDTH 160   // LED display width, in pixels
#define SCREEN_HEIGHT 128  // LED display height, in pixels

#define BACKGROUND_COLOR ST77XX_BLACK
#define TEXT_COLOR ST77XX_WHITE
#define CLEAR_DISPLAY display.fillScreen(BACKGROUND_COLOR)
#define UPDATE_DISPLAY  // No update needed

Adafruit_ST7735 display = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
#else

// SSD1306
#include <Wire.h>
#include <Adafruit_SSD1306.h>  //https://github.com/adafruit/Adafruit_SSD1306
#define SS_PIN 4
#define RST_PIN 5
// Declaration for SSD1306 display connected using software I2C pins are(22 SCL, 21 SDA)
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET 0      // Reset pin # (or -1 if sharing Arduino reset pin)

#define BACKGROUND_COLOR BLACK
#define TEXT_COLOR WHITE
#define CLEAR_DISPLAY display.clearDisplay()
#define UPDATE_DISPLAY display.display()

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif
//************************************************************************
//************************************************************************
#define SS_PIN 21
#define RST_PIN 22

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
Preferences preferences;

WebServer server(80);

//************************************************************************
/* Set these to your desired credentials. */
String ssid = "vinces_house";
String password;
String device_token;
String URL;
//************************************************************************
int timezone = -5 * 3600;  //Replace "x" your timezone.
int time_dst = 0;
String getData, Link;
String OldCardID = "";
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
//*************************Biometric Icons*********************************
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
//************************************************************************
void handleNotFound();
void handleRoot();
void connectToWiFi(const char *ssid, const char *pw);
void drawGraph();
const int led = 2;
bool apmode = false;

#define TIME_WIDTH 128
//************************************************************************

void setup() {
  Serial.begin(115200);
  preferences.begin("rfid-scanner", false);
  ssid = preferences.getString("ssid", "");
  password = preferences.getString("password", "");
  URL = preferences.getString("URL", "");


  //-----------initialize display-------------
#ifdef USING_ST7735
  display.initR(INITR_BLACKTAB);  // Init ST7735S chip, black tab
  display.setRotation(3);
#else
  SPI.begin();                                       // Init SPI bus
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  // you can delet these three lines if you don't want to get the Adfruit logo appear
  UPDATE_DISPLAY;
  delay(2000);  // Pause for 2 seconds
  display.clearDisplay();
#endif

  mfrc522.PCD_Init();  // Init MFRC522 card

  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  //---------------------------------------------
  connectToWiFi(ssid.c_str(), password.c_str());
  //---------------------------------------------
  configTime(timezone, time_dst, "pool.ntp.org", "time.nist.gov");
}
//************************************************************************
void loop() {
  //check if there's a connection to Wi-Fi or not
  if (!WiFi.isConnected() && !apmode) {
    connectToWiFi(ssid.c_str(), password.c_str());  //Retry to connect to Wi-Fi
  } else {
    server.handleClient();
  }
  //---------------------------------------------
  if (millis() - previousMillis1 >= 1000) {
    previousMillis1 = millis();

    time_t now = time(nullptr);
    struct tm *p_tm = localtime(&now);
    CLEAR_DISPLAY;
    display.setTextSize(1);            // Normal 2:2 pixel scale
    display.setTextColor(TEXT_COLOR);  // Draw white text
    display.setCursor(0, 0);
    display.print(p_tm);
    //display.print("    ");
    //Serial.println(p_tm);
    display.setTextSize(4);            // Normal 2:2 pixel scale
    display.setTextColor(TEXT_COLOR);  // Draw white text
    display.setCursor((SCREEN_WIDTH - TIME_WIDTH) / 2, 21);
    if ((p_tm->tm_hour) < 10) {
      display.print("0");
      display.print(p_tm->tm_hour);
    } else display.print(p_tm->tm_hour);
    display.print(":");
    if ((p_tm->tm_min) < 10) {
      display.print("0");
      display.println(p_tm->tm_min);
    } else display.println(p_tm->tm_min);
    //display.print(" ");
    UPDATE_DISPLAY;
  }
  //   SendCardID("101759940994");
  /*
1087631147.3398438
loginFlynn, Gabe
Scan Card

397499769.5078125
loginJacobson, Vince

 101759940994: 17 b1 5d 79 (82) ARDUINO 2317793121 = 8A26B761
 278433573719: 40 d3 ef 2b (57) ARDUINO 6421123943
*/
  //   delay(5000);
  //---------------------------------------------
  if (millis() - previousMillis2 >= 15000) {
    previousMillis2 = millis();
    OldCardID = "";
  }
  delay(50);

  //---------------------------------------------
  //look for new card
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;  //got to start of loop if there is no card present
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;  //if read card serial(0) returns 1, the uid struct contians the ID of the read card.
  }
  String CardID = "";
  int cardIdNum = 0;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    CardID += mfrc522.uid.uidByte[i];
    cardIdNum = cardIdNum * 256 + mfrc522.uid.uidByte[i];
    //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    // Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  char buffer[32];
  sprintf(buffer, "%d", cardIdNum);

  //---------------------------------------------
  if (CardID == OldCardID) {
    return;
  } else {
    OldCardID = CardID;
  }

  //---------------------------------------------
  //  Serial.println(CardID);
  SendCardID(buffer);
  delay(1000);
  CLEAR_DISPLAY;
}
//************send the Card UID to the website*************
void SendCardID(String Card_uid) {
  Serial.println("Sending the Card ID");
  if (WiFi.isConnected()) {
    HTTPClient http;  //Declare object of class HTTPClient
    //GET Data
    getData = "?card_uid=" + String(Card_uid) + "&device_token=" + String(device_token);  // Add the Card ID to the GET array in order to send it
    //GET methode
    Link = URL + getData;
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
//********************connect to the WiFi******************
void connectToWiFi(const char *ssid, const char *pw) {
  WiFi.mode(WIFI_OFF);  //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pw);

  CLEAR_DISPLAY;
  display.setTextSize(1);            // Normal 1:1 pixel scale
  display.setTextColor(TEXT_COLOR);  // Draw white text
  display.setCursor(0, 0);           // Start at top-left corner
  display.print(F("Connecting to \n"));
  display.setCursor(0, 60);
  display.setTextSize(2);
  display.print(ssid);
  display.drawBitmap((SCREEN_WIDTH - Wifi_start_width) / 2, 10, Wifi_start_bits, Wifi_start_width, Wifi_start_height, TEXT_COLOR);
  UPDATE_DISPLAY;
  int retries = 5;
  while (WiFi.status() != WL_CONNECTED && retries-- > 0) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() != WL_CONNECTED) {
    if (!WiFi.softAP("rfid_scanner_setup", "")) {
      Serial.println("Soft AP creation failed.");
      while (1)
        ;
    }
    Serial.println("Access point: rfid_scanner_setup");
    Serial.println(WiFi.softAPIP());
    apmode = true;
  } else {
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
  if (MDNS.begin("RFID Scanner")) {
    Serial.println("MDNS responder started");
  }
  server.begin();
  MDNS.addService("http", "tcp", 80);

  server.on("/", handleRoot);
  server.on("/test.svg", drawGraph);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  delay(3000);
}
//=======================================================================
char temp[4000];

void handleRoot() {
 for (uint8_t i = 0; i < server.args(); i++) {
    Serial.println(" " + server.argName(i) + ": " + server.arg(i));
  }
  digitalWrite(led, 1);
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, sizeof(temp) ,R"(
  <html>
<head>
  <!-- <meta http-equiv='refresh' content='5' /> -->
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
    <h1>RFID Scanner Configuration</h1>
    <p>Uptime: %02d:%02d:%02d</p>
    <form method="POST">
        <label for="ssid">SSID:</label>
        <input type="text" id="ssid" name="ssid" value="%s"><br>
        <label for="pw">Password:</label>
        <input type="text" id="pw" name="pw" value="%s"><br><br>
        <input type="submit" value="Submit">
      </form> 
      </body>
</html>
)",

           hr, min % 60, sec % 60, "", "");
  server.send(200, "text/html", temp);
  digitalWrite(led, 0);
}

void handleNotFound() {
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

void drawGraph() {
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x += 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";

  server.send(200, "image/svg+xml", out);
}
