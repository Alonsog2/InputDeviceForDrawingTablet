#include <MIDIUSB.h>       


#define MIDICHANNEL_CTRL 1   // Canal por el que se transmiten los cambios de control (del footSwitch) 
#define MIDINUMBER_CTRL  45  // Nº de controlador
#define MIDIVALUE_CTRL   64  // Valor del controlador On


boolean MIDImode = START_IN_MIDI_MODE;


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



byte MIDIvalRotaryEncoders[N_ENCODERS] = {
  64,64,64
};



void sendNoteOn_USB(byte pitch, byte velocity, byte channel) {               // enviar notaOn al PC por USB del Leonardo
  midiEventPacket_t midiPack = {0x09, 0x90 | (--channel), pitch, velocity};  // MIDIUSB numera los canales del 0 al 15
  MidiUSB.sendMIDI(midiPack);
  MidiUSB.flush();
}



void sendNoteOff_USB(byte pitch, byte velocity, byte channel ) {            // enviar notaOff al PC por USB del Leonardo
  midiEventPacket_t midiPack = {0x08, 0x80 | (--channel), pitch, velocity}; // MIDIUSB numera los canales del 0 al 15
  MidiUSB.sendMIDI(midiPack);
  MidiUSB.flush();
}



void sendCtrlChange_USB(byte control, byte value, byte channel) {
  midiEventPacket_t event = {0x0B, 0xB0 | (--channel), control, value};     // MIDIUSB numera los canales del 0 al 15
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}



void sendMIDIreset(){
  midiEventPacket_t event = {0xFF, 0, 0, 0};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

// MIDI_CHANNEL = 0,           // Channel (1-16/0=global)
//  MIDI_CC_OR_NOTE,            // CC or Note (0,1)
//  MIDI_MOMENTARY,             // momentary or toggle (0/1)
//  MIDI_nCC_OR_NOTEPITCH,      // nCC or NotePitch
//  MIDI_ValMin_OR_velNoteOff,  // valMinCC or noteOff
//  MIDI_ValMax_OR_velNoteOn

void pressedMIDIKey(int keyIndex){
  Serial.println(F("Midi pressed")); 
  byte channel = (actionsMIDI[keyIndex][MIDI_CHANNEL] == 0) ? GLOBAL_MIDI_CHANNEL : actionsMIDI[keyIndex][0];

  if (actionsMIDI[keyIndex][MIDI_CC_OR_NOTE] == 1) {                                               // Note
    boolean bSendNote = true;
    byte note = actionsMIDI[keyIndex][MIDI_nCC_OR_NOTEPITCH];
    if  (actionsMIDI[keyIndex][MIDI_MOMENTARY] != 0) {                                             // if toggle...
      MIDInoteStatus[note] = ! MIDInoteStatus[note];
      bSendNote = MIDInoteStatus[note];
      if (!bSendNote) {
        sendNoteOff_USB(note,actionsMIDI[keyIndex][MIDI_ValMin_OR_velNoteOff], channel); 
      }
    }
    if  (bSendNote) {
      sendNoteOn_USB(note,actionsMIDI[keyIndex][MIDI_ValMax_OR_velNoteOn], channel);      
    }
    
  } else {                                                                                         // CC 
    sendCtrlChange_USB(actionsMIDI[keyIndex][MIDI_nCC_OR_NOTEPITCH],actionsMIDI[keyIndex][MIDI_ValMax_OR_velNoteOn], channel);  
  }
}



void releasedMIDIKey(int keyIndex){
  Serial.println(F("Midi released")); 
  byte channel = (actionsMIDI[keyIndex][MIDI_CHANNEL] == 0) ? GLOBAL_MIDI_CHANNEL : actionsMIDI[keyIndex][0];
  if (actionsMIDI[keyIndex][MIDI_CC_OR_NOTE] == 1) {                                                // Note
    if  (actionsMIDI[keyIndex][MIDI_MOMENTARY] == 0) {                                              // Send NoteOff only if 'momentary' mode for this note
      sendNoteOff_USB(actionsMIDI[keyIndex][MIDI_nCC_OR_NOTEPITCH],actionsMIDI[keyIndex][MIDI_ValMin_OR_velNoteOff], channel);      
    }
  } else {                                                                                          // CC
    sendCtrlChange_USB(actionsMIDI[keyIndex][MIDI_nCC_OR_NOTEPITCH],actionsMIDI[keyIndex][MIDI_ValMin_OR_velNoteOff], channel);   
  }
}
