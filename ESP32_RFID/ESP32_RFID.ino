/* -----------------------------------------------------------------------------
  - Project: RFID attendance system using ESP32
  - Author:  https://www.youtube.com/ElectronicsTechHaIs
  - Date:  6/03/2020
   -----------------------------------------------------------------------------
  This code was created by Electronics Tech channel for 
  the RFID attendance project with ESP32.
   ---------------------------------------------------------------------------*/

#include "RfidLcd.h"
#include "RfidSound.h"

//*******************************libraries********************************
#include "ESP32_RFID.h"
//RFID-----------------------------
#include <SPI.h>
#include <MFRC522.h>
//************************************************************************
#define SS_PIN 21
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN);  // Create MFRC522 instance.

//************************************************************************
int timezone = -5 * 3600;  //Replace "x" your timezone.
int time_dst = 0;
String OldCardID = "";
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
//************************************************************************

void setup() {
  Serial.begin(115200);

  rfid.PCD_Init();  // Init MFRC522 card
  RfidLcdSetup();

  RfidSoundLaunch();

  RfidWiFiSetup();
  configTime(timezone, time_dst, "pool.ntp.org", "time.nist.gov");
}

//************************************************************************
void loop() {
  RfidWiFiTick();
  RfidLcdTick();
  if (RfidWiFiApMode()) return;
  //---------------------------------------------
  if (millis() - previousMillis1 >= 1000) {
    previousMillis1 = millis();
    LcdDisplayTime();
  }
  //---------------------------------------------
  if (millis() - previousMillis2 >= 15000) {
    previousMillis2 = millis();
    OldCardID = "";
  }
  delay(50);

  //---------------------------------------------
  //look for new card
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;
  //if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) 
  {
    // New card, successfully read. The uid struct contians the ID of the new card.
    char buffer[32];
    // Format as 4 bytes in lower case hex 
    sprintf(buffer, "%02x%02x%02x%02x", rfid.uid.uidByte[0],
            rfid.uid.uidByte[1], rfid.uid.uidByte[2], rfid.uid.uidByte[3]);

    String CardID = buffer;

    if (CardID != OldCardID) {
      OldCardID = CardID;

      //  Serial.println(CardID);
      SendCardID(buffer);
      delay(1000);
      LcdDisplayEndNotice();
    }
  }
}
