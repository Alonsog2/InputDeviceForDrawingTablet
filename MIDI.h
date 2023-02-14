
#include <MIDIUSB.h>       


boolean MIDImode = ((DeviceModel == MIDI_ONLY) || (DeviceModel == MIDI_AND_KEYBOARD));


byte MIDInoteStatus[16] = {                              // 16bytes*8 = 128 notes (One bit for the status of each note)
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};


const char NOTE_NAMES[] = "C C#D D#E F F#G G#A A#B ";
char buffNotation[] = "1234";                           // For displaying musical notation I.E. C#-1
