#pragma once

#include "Notify.h"

/** Extension of notify class to add RFID specific notifications
 *
 */
class RfidNotify : public Notify {
public:
  RfidNotify(AudioNotify *audio)
    : Notify(audio) {}

  void Arrive(String user_name) {
    LcdDisplayNotifyArrive(user_name);
    if (audio) {
      audio->Arrive();
    }
  }
  void Depart(String user_name) {
    LcdDisplayNotifyDepart(user_name);
    if (audio) {
      audio->Depart();
    }
  }
  void Available(String message) {
    LcdDisplayNotifyAvailable(message);
  }
  void Successful(String message) {
    LcdDisplayNotifySuccessful(message);
  }
};