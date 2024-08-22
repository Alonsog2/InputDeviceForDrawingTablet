#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

#define I2C_ADDRESS 0x3C     // 0X3C+SA0 - 0x3C or 0x3D
#define RST_PIN -1           // Define proper RST_PIN if required.


SSD1306AsciiAvrI2c oled;

#define INIT_LINE_AREA2 2

#define MSG_NORMAL_MODE "4x4Keys"
#define MAX_COL 128
#define MAX_ROW 64



void initDisplay() {
  #if RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
  #else // RST_PIN >= 0
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
  #endif // RST_PIN >= 0

  oled.setFont(fixed_bold10x15);
  //oled.set2X();
  
  clearAllDisplay();
  
  displayStatus();
}



void clearAllDisplay(){
  oled.clear();
}



void clearAreaUPDisplay() {
  oled.clear(0, MAX_COL, 0, INIT_LINE_AREA2 - 1);
}



void clearAreaBottomDisplay() {
  oled.clear(0, MAX_COL, INIT_LINE_AREA2, MAX_ROW);
}



void displayKeyLabel(String label) {
  clearAreaBottomDisplay();
  oled.setCursor(0, INIT_LINE_AREA2);
  oled.print(label);

  if (testMode) {
    if (sendInfoToComputerInTestMode) {
      Keyboard.println(label);
    }
    Serial.println(label);
  }
}


void displayStatus() {
  char msg[11] = MSG_NORMAL_MODE;

  if (bSystemLocked) {
    strcpy(msg, "Locked");
  }
  
  if (testMode) {
    strcpy(msg, "Test");
    if (localShiftMode != NO_LOCAL_SHIFT) {
      strcpy(msg + 4, " SHIFT");
    }
  } else {
    switch (localShiftMode) {
      case LOCAL_SHIFT_TEMP:
        strcpy(msg, "Tmp.SHIFT");
        break;
      case LOCAL_SHIFT_LOCKED:
        strcpy(msg, "SHIFT");
        break;
    }
  }

  if (MIDImode) {
    strcpy(msg, "MIDI");
    oled.clear();
  }

  clearAreaUPDisplay();
  oled.setCursor(0, 0);
  oled.print(msg);

  if (testMode) {
    if (sendInfoToComputerInTestMode) {
      Keyboard.print(F("Status "));
      Keyboard.println(msg);
    }
    Serial.print(F("Status "));
    Serial.println(msg);
  }
}

void displayMIDIInfo(byte cmd, byte Data1, byte Data2, byte channel) {
  oled.setCursor(0, INIT_LINE_AREA2);
  clearAreaBottomDisplay();
  if (cmd == 0xB0) {                       // CC message
    oled.print(F("CC Ch"));
    oled.print(channel);
    oled.print(F("\nCtrl "));
    oled.print(Data1);
    oled.print(F("\nVal "));
    oled.print(Data2);
  } else {
    oled.print(F("N. Ch"));
    oled.print(channel);
    oled.print(F("\nNote "));
    notePitch2Notation(Data1);                 // fill buffer with notation
    oled.print(buffNotation);
    oled.print(F("\nVel "));
    oled.print(Data2);
  }

  if (testMode || true) {
    Serial.print (F("Midi cmd Hex "));
    Serial.println(cmd, HEX);
    Serial.print(F("Ch: "));
    Serial.println(channel);
    Serial.println(Data1);
    Serial.println(Data2);
  }

}
