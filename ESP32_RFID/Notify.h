#pragma once

#include "AudioNotify.h"
#include "LcdNotify.h"

/** Notify user in whatever way makes sense for this system
 *
 * Default implementation uses LCD Display and optionall audio
 */
class Notify {
public:
  AudioNotify *audio;
  Notify(AudioNotify *audio)
    : audio(audio) {}
  void begin(void) {
    LcdSetup();
  }
  void Tick(void) {
    LcdTick();
  }
  void Launch() {
    LcdDisplayLaunch();
    if (audio) {
      audio->Launch();
    }
  }
  void Time() {
    LcdDisplayTime();
  }
  void EndNotice() {
    LcdDisplayEndNotice();
  }
  void Error(String message) {
    LcdDisplayNotifyError(message);
  }
  void Connecting(String ssid, bool graphic) {
    LcdDisplayConnecting(ssid, graphic);
  }
  void Connected(IPAddress ip) {
    LcdDisplayConnected(ip);
  }
  void ApMode(String ssid, IPAddress ip) {
    LcdDisplayApMode(ssid, ip);
  }
};

