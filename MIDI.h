#include <MIDIUSB.h>       


#define MIDICHANNEL_CTRL 1   // Canal por el que se transmiten los cambios de control (del footSwitch) 
#define MIDINUMBER_CTRL  45  // Nº de controlador
#define MIDIVALUE_CTRL   64  // Valor del controlador On



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



boolean MIDImode = START_IN_MIDI_MODE;



void pressedMIDIKey(char key){
  Serial.println(F("Midi pressed")); 
}



void releasedMIDIKey(char key){
  Serial.println(F("Midi released")); 
}



//void prbON() {
//sendCtrlChange_USB(MIDINUMBER_CTRL, MIDIVALUE_CTRL, MIDICHANNEL_CTRL);              //soltar pedal
//MidiUSB.flush();
//}
//
//void prbOFF(){
//sendCtrlChange_USB(MIDINUMBER_CTRL, 0, MIDICHANNEL_CTRL);              //soltar pedal
//MidiUSB.flush();
//}
