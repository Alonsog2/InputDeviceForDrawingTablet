
#include <MIDIUSB.h>       


#define MIDICHANNEL_CTRL 1   // Canal por el que se transmiten los cambios de control (del footSwitch) 
#define MIDINUMBER_CTRL  45  // Nº de controlador
#define MIDIVALUE_CTRL   64  // Valor del controlador On


boolean MIDImode = ((DeviceModel == MIDI_ONLY) || (DeviceModel == MIDI_AND_KEYBOARD));


boolean MIDInoteStatus[128] = {  
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};



const char NOTE_NAMES[] = "C C#D D#E F F#G G#A A#B ";
char buffNotation[] = "1234";                                    // I.E. C#-1
