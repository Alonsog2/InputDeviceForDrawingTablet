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
  String msg = MSG_NORMAL_MODE;
  if (testMode) {
    msg = "Test";
    if (localShiftMode != NO_LOCAL_SHIFT) {
      msg += " SHIFT";
    }
  } else {
    switch (localShiftMode) {
      case LOCAL_SHIFT_TEMP:
        msg = "Temp.SHIFT";
        break;
      case LOCAL_SHIFT_LOCKED:
        msg = "SHIFT";
        break;
    }
  }

  if (MIDImode) {
    msg="MIDI mode";
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
    msg = "Status " + msg;
    if (sendInfoToComputerInTestMode) {
      Keyboard.println(msg);
    }
    Serial.println(msg);
  }
}



void displayMIDIInfo(byte cmd, byte Data1, byte Data2, byte channel){
  String msg;
  if (cmd == 0xB0) {                       // CC message
    msg = "CC Ch" +String(channel);
    msg = msg + "\nCtrl ";
    msg = msg + String(Data1);
    msg = msg + "\nVal ";
  } else {
    msg = "N." + String((cmd == 0x80) ? "OFF" : "ON");
    msg = msg + " Ch";
    msg = msg + String(channel);
    msg = msg + "\nNote ";
    msg = msg + String(Data1);
    msg = msg + "\nVel ";
  }
  msg = msg + String(Data2);
Serial.println(msg);
  
  if (displayPresent) {
    clearAreaBottomDisplay();
    display.setCursor(0, INIT_LINE_AREA2);
    display.print(msg);
    display.display();
  }

  if (testMode) {
    if (sendInfoToComputerInTestMode) {
      Keyboard.println(msg);
    }
    Serial.println(msg);
  }
}
