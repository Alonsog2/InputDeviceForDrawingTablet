
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define INIT_LINE_AREA2 17

#define MSG_NORMAL_MODE "4x4Keys"

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32 ??
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void initDisplay() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    Serial.println(F("Display alloc fail"));
  } else {
    Serial.println(F("DisplayOK"));
    displayPresent = true;
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.cp437(true);                                     // Use full 256 char 'Code Page 437' font
    displayStatus();
  }
}



void clearAreaUPDisplay(boolean refreshDisplay = false) {
  if (displayPresent) {
    display.fillRect(0, 0, display.width(), INIT_LINE_AREA2 - 1, SSD1306_BLACK);
    if (refreshDisplay) {
      display.display();
    }
  }
}



void clearAreaBottomDisplay(boolean refreshDisplay = false) {
  if (displayPresent) {
    display.fillRect(0, INIT_LINE_AREA2, display.width(), display.height(), SSD1306_BLACK);
    if (refreshDisplay) {
      display.display();
    }
  }
}



void displayKeyLabel(String label) {
  if (displayPresent) {
    clearAreaBottomDisplay();
    display.setCursor(0, INIT_LINE_AREA2);
    display.print(label);
    display.display();
  }

  if (testMode) {
    if (sendInfoToComputerInTestMode) {
      Keyboard.println(label);
    }
    Serial.println(label);
  }
}



void displayStatus() {
  char msg[11] = MSG_NORMAL_MODE;
  if (testMode) {
    strcpy(msg, "Test");
    if (localShiftMode != NO_LOCAL_SHIFT) {
      strcpy(msg + 4," SHIFT");
    }
  } else {
    switch (localShiftMode) {
      case LOCAL_SHIFT_TEMP:
        strcpy(msg,"Tmp.SHIFT");
        break;
      case LOCAL_SHIFT_LOCKED:
        strcpy(msg,"SHIFT");
        break;
    }
  }

  if (MIDImode) {
    strcpy(msg,"MIDI");
    if (displayPresent) {
      display.clearDisplay();
    }
  }
  
  if (displayPresent) {
    clearAreaUPDisplay();
    display.setCursor(0, 0);
    display.print(msg);
    display.display();
  }

  if (testMode) {
    if (sendInfoToComputerInTestMode) {
      Keyboard.print(F("Status "));
      Keyboard.println(msg);
    }
    Serial.print(F("Status "));
    Serial.println(msg);
  }
}

void displayMIDIInfo(byte cmd, byte Data1, byte Data2, byte channel){
  if (displayPresent) {
    display.setCursor(0, INIT_LINE_AREA2);
    clearAreaBottomDisplay();
    if (cmd == 0xB0) {                       // CC message
      display.print(F("CC Ch"));
      display.print(channel);
      display.print(F("\nCtrl "));
      display.print(Data1);
      display.print(F("\nVal "));
      display.print(Data2);
    } else {
      display.print(F("N. Ch"));
      display.print(channel);
      display.print(F("\nNote "));
      notePitch2Notation(Data1);                 // fill buffer with notation
      display.print(buffNotation); 
      display.print(F("\nVel "));
      display.print(Data2);
    }
    display.display();
  }

  if (testMode || true) {
    Serial.print (F("Midi cmd Hex "));
    Serial.println(cmd,HEX);
    Serial.print(F("Ch: "));
    Serial.println(channel);
    Serial.println(Data1);
    Serial.println(Data2);
  }
    
}
