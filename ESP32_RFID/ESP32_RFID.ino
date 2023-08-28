/* -----------------------------------------------------------------------------
  - Project: RFID attendance system using ESP32
  - Author:  https://www.youtube.com/ElectronicsTechHaIs
  - Date:  6/03/2020
   -----------------------------------------------------------------------------
  This code was created by Electronics Tech channel for 
  the RFID attendance project with ESP32.
   ---------------------------------------------------------------------------*/

#include "RfidSound.h"

//*******************************libraries********************************
#include "ESP32_RFID.h"
//RFID-----------------------------
#include <SPI.h>
#include <MFRC522.h>
//DISPLAY-----------------------------
#ifdef USING_ST7735
Adafruit_ST7735 display = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
#else
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif
//************************************************************************
#define SS_PIN 21
#define RST_PIN 22

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.

//************************************************************************
int timezone = -5 * 3600;  //Replace "x" your timezone.
int time_dst = 0;
String OldCardID = "";
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
//************************************************************************

#define TIME_WIDTH 128
//************************************************************************

void setup() {
  Serial.begin(115200);

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
  CLEAR_DISPLAY;
#endif

  mfrc522.PCD_Init();  // Init MFRC522 card

  sound_startup();

  RfidWiFiSetup();
  configTime(timezone, time_dst, "pool.ntp.org", "time.nist.gov");
}
//************************************************************************
void loop() {
  RfidWiFiTick();
  if (RfidWiFiApMode()) return;
  //---------------------------------------------
  if (millis() - previousMillis1 >= 1000) {
    previousMillis1 = millis();

    time_t now = time(nullptr);
    struct tm *p_tm = localtime(&now);
    CLEAR_DISPLAY;
    display.setTextSize(1);            // Normal 2:2 pixel scale
    display.setTextColor(TEXT_COLOR);  // Draw white text
    display.setCursor(6, 3);
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
  char buffer[32];
  sprintf(buffer, "%02x%02x%02x%02x", mfrc522.uid.uidByte[0], 
    mfrc522.uid.uidByte[1], mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3]);

  String CardID = buffer;

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
