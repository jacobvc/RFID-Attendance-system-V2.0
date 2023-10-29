#pragma once

#include "WiFiServer.h"

/** Extends WifiServer to add RFID specific web pages 
 *
 */
class RfidWifiServer: public WifiServer {
  public:
  static RfidWifiServer *exec;
  RfidWifiServer(Preferences& preferences, Notify *notify, String mdnsName) 
    : WifiServer(preferences, notify, mdnsName) {
      exec = this;
  }

  void connectToWiFi(const char *ssid, const char *pw);
  void root();
  void NotFound();
  void TimezoneJs();
};
