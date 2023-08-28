/*
 *
 * Defines / Macros 
 *
 */

 #include <Arduino.h>

#define USING_ST7735
#define CONFIG_AP_SSID "rfid_config"

#define BACKGROUND_COLOR WHITE
#define TEXT_COLOR BLACK



extern void RfidLcdSetup(void);
extern void RfidLcdTick(void);

extern void LcdDisplayTime();
extern void LcdDisplayEndNotice();
extern void LcdDisplayNotifyArrive(String user_name);
extern void LcdDisplayNotifyDepart(String user_name);
extern void LcdDisplayNotifyAvailable(String message);
extern void LcdDisplayNotifySuccessful(String message);
extern void LcdDisplayNotifyError(String message);

extern void LcdDisplayConnecting(String ssid);
extern void LcdDisplayConnected(IPAddress ip);
extern void LcdDisplayApMode(String ssid, IPAddress ip);

