#include "RfidSound.h"
#include <arduino.h>

void sound_arrive()
{
  int melody[] = {
    NOTE_C3, NOTE_G4
  };

  unsigned char durations[] = {
    6, 8
  };

 for (int thisNote = 0; thisNote < sizeof(durations); thisNote++) {
    int noteDuration = 1000 / durations[thisNote];
    tone(BUZZZER_PIN, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZZER_PIN);
  }
}
void sound_depart()
{
  int melody[] = {
    NOTE_G4, NOTE_C3
  };

  unsigned char durations[] = {
    6, 8
  };

 for (int thisNote = 0; thisNote < sizeof(durations); thisNote++) {
    int noteDuration = 1000 / durations[thisNote];
    tone(BUZZZER_PIN, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZZER_PIN);
  }
}

void sound_startup()
{
  int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
  };

  unsigned char durations[] = {
    4, 8, 8, 4, 4, 4, 4, 4
  };

 for (int thisNote = 0; thisNote < sizeof(durations); thisNote++) {
    int noteDuration = 1000 / durations[thisNote];
    tone(BUZZZER_PIN, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZZER_PIN);
  }
}