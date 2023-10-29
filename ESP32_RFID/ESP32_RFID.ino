#include "configuration.h"

#include "RfidWifiServer.h"

#include <SPI.h>
#include <MFRC522.h>

#include "RfidNotify.h"

Preferences preferences;
String device_token;
int backlight;
String url;

MFRC522 rfid(RC522_CS_PIN, RC522_RST_PIN);  // Create MFRC522 instance.

AudioNotify audio;
RfidNotify notify(&audio);

RfidWifiServer wifiServer(preferences, &notify, CONFIG_MDNS_NAME);

//************************************************************************
String OldCardID = "";
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
//************************************************************************

void setup() {
  Serial.begin(115200);

  preferences.begin("rfid-scanner", false);
  device_token = preferences.getString("device_token", "");
  backlight = preferences.getInt("backlight", 180);
  url = preferences.getString("url", "");

  notify.begin();

  SPI.begin(); // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522 card

  notify.Launch();

  wifiServer.begin();
}

//************************************************************************
void loop() {
  wifiServer.Tick();
  notify.Tick();

  if (wifiServer.ApMode()) return;
  //---------------------------------------------
  if (millis() - previousMillis1 >= 1000) {
    previousMillis1 = millis();
    notify.Time();
 }
  //---------------------------------------------
  if (millis() - previousMillis2 >= 15000) {
    previousMillis2 = millis();
    OldCardID = "";
  }
  delay(50);

  //---------------------------------------------
  //look for new card
  if ( ! rfid.PICC_IsNewCardPresent()) {
    //Serial.println("Not New");
    return;
  }
  // Verify if the NUID has been read
  if ( ! rfid.PICC_ReadCardSerial()) {
    Serial.println("Read Fail");
    return;
  }
  //if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) 
  {
    // New card, successfully read. The uid struct contians the ID of the new card.
    char buffer[32];
    // Format as 4 bytes in lower case hex 
    sprintf(buffer, "%lu", rfid.uid.uidByte[0] << 24
      | rfid.uid.uidByte[1] << 16 | rfid.uid.uidByte[2] << 8 | rfid.uid.uidByte[3]);
//    sprintf(buffer, "%02x%02x%02x%02x", rfid.uid.uidByte[0],
//            rfid.uid.uidByte[1], rfid.uid.uidByte[2], rfid.uid.uidByte[3]);

    String CardID = buffer;

    if (CardID != OldCardID) {
      OldCardID = CardID;

      //  Serial.println(CardID);
    //GET Data
    String getData = "?card_uid=" + String(buffer) + "&device_token=" + String(device_token);  // Add the Card ID to the GET array in order to send it
      String payload = wifiServer.get(getData);
      if (payload.substring(0, 5) == "login") {
        String user_name = payload.substring(5);
        notify.Arrive(payload.substring(5));
      } else if (payload.substring(0, 6) == "logout") {
        notify.Depart(payload.substring(6));
      } else if (payload == "succesful") {
        notify.Successful(payload);
      } else if (payload == "available") {
        notify.Available(payload);
      } else {
        notify.Error(payload);
      }

      delay(1000);
      notify.EndNotice();
    }
  }
}
