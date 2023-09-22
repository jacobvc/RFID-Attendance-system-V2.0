/*
 *
 * Defines / Macros 
 *
 */

#include <Arduino.h>

#include "HwConfig.h"

#ifdef NO_DISPLAY
#define RfidLcdSetup()
#define RfidLcdTick()

#define LcdDisplayLaunch()
#define LcdDisplayTime()
#define LcdDisplayEndNotice()
#define LcdDisplayNotifyArrive(user_name)
#define LcdDisplayNotifyDepart(user_name)
#define LcdDisplayNotifyAvailable(message)
#define LcdDisplayNotifySuccessful(message)
#define LcdDisplayNotifyError(message)

#define LcdDisplayConnecting(ssid)
#define LcdDisplayConnected(ip)
#define LcdDisplayApMode(ssid, ip)
#else
extern int backlight;
extern void RfidLcdSetup(void);
extern void RfidLcdTick(void);

extern void LcdDisplayLaunch();
extern void LcdDisplayTime();
extern void LcdDisplayEndNotice();
extern void LcdDisplayNotifyArrive(String user_name);
extern void LcdDisplayNotifyDepart(String user_name);
extern void LcdDisplayNotifyAvailable(String message);
extern void LcdDisplayNotifySuccessful(String message);
extern void LcdDisplayNotifyError(String message);

extern void LcdDisplayConnecting(String ssid, bool graphic);
extern void LcdDisplayConnected(IPAddress ip);
extern void LcdDisplayApMode(String ssid, IPAddress ip);
#endif
