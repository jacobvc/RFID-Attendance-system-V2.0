#include <Preferences.h>
#include <time.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

/*
 *
 * Defines / Macros 
 *
 */

#define USING_ST7735
#define CONFIG_AP_SSID "rfid_config"

#define BACKGROUND_COLOR WHITE
#define TEXT_COLOR BLACK

/*
 *
 * Display normalization  (for ST7735 vs SSD1306)
 *
 */

#include <Adafruit_GFX.h>  //https://github.com/adafruit/Adafruit-GFX-Library
#ifdef USING_ST7735
// ST7735
#include <Adafruit_ST7735.h>  //https://github.com/adafruit/Adafruit_ST7735
#define TFT_CS 5
#define TFT_RST 4  // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC 2

#define SCREEN_WIDTH 160   // LED display width, in pixels
#define SCREEN_HEIGHT 128  // LED display height, in pixels

#define BLACK ST77XX_BLACK
#define WHITE ST77XX_WHITE
#define RED ST77XX_RED
#define BLUE ST77XX_BLUE
#define GREEN ST77XX_GREEN

#define CLEAR_DISPLAY display.fillScreen(BACKGROUND_COLOR)
#define UPDATE_DISPLAY  // No update needed
extern Adafruit_ST7735 display;
#else
// SSD1306
#include <Wire.h>
#include <Adafruit_SSD1306.h>  //https://github.com/adafruit/Adafruit_SSD1306
#define SS_PIN 4
#define RST_PIN 22
// Declaration for SSD1306 display connected using software I2C pins are(22 SCL, 21 SDA)
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define OLED_RESET 0      // Reset pin # (or -1 if sharing Arduino reset pin)

#define CLEAR_DISPLAY display.clearDisplay()
#define UPDATE_DISPLAY display.display()

extern Adafruit_SSD1306;
#endif

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

