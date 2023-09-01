#include <Preferences.h>
#include <time.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#define CONFIG_AP_SSID "rfid_config"  // SSID to use in AP Mode

/*
 *
 * Function Prototypes
 *
 */
extern void connectToWiFi(const char *ssid, const char *pw);
extern void SendCardID(String Card_uid);

extern void RfidWiFiSetup(void);
extern void RfidWiFiTick(void);
extern bool RfidWiFiApMode(void);

