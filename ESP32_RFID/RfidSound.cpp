#include "RfidSound.h"
#include <arduino.h>

#define COUNTOF(arr) (sizeof(arr) / sizeof(arr[0]))
typedef struct {
  int note;
  int duration;
} tune;

static void play(tune *notes, int count)
{
 for (int thisNote = 0; thisNote < count; thisNote++) {
    int noteDuration = 1000 / notes[thisNote].duration;
    tone(BUZZZER_PIN, notes[thisNote].note, noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZZER_PIN);
  }
}

void RfidSoundArrive()
{
  tune diddy[] = { { NOTE_C3, 6 }, { NOTE_G4, 8}  };
  play(diddy, COUNTOF(diddy));
}
void RfidSoundDepart()
{
  tune diddy[] = { { NOTE_G4, 6 }, { NOTE_C3, 8}  };
  play(diddy, COUNTOF(diddy));
}

void RfidSoundLaunch()
{
  tune diddy[] = {
    { NOTE_C4, 4 }, { NOTE_G3, 8 }, { NOTE_G3, 8 }, { NOTE_A3, 4 }, { NOTE_G3, 4 }, { 0, 4 }, { NOTE_B3, 4 }, { NOTE_C4, 4 }
  };
  play(diddy, COUNTOF(diddy));
}