#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define INIT_LINE_AREA2 16

#define MSG_NORMAL_MODE "4x4Keyboard"

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32 ??
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


boolean displayPresent = false;


void initDisplay() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    Serial.println(F("SSD1306 allocation failed"));
  } else {
    displayPresent = true;
    display.clearDisplay();
    display.setTextSize(2);              
    display.setTextColor(SSD1306_WHITE); 
    display.cp437(true);                                     // Use full 256 char 'Code Page 437' font
  
    display.setCursor(0,0);                                  // Start at top-left corner
   
    display.display();
  }
}



void clearAreaUPDisplay(boolean refreshDisplay = false){
  if (displayPresent) {
    display.fillRect(0, 0, display.width(), INIT_LINE_AREA2-1, SSD1306_BLACK);
    if (refreshDisplay) {
      display.display(); 
    } 
  }
}



void clearAreaBottomDisplay(boolean refreshDisplay = false){
  if (displayPresent) {
    display.fillRect(0, INIT_LINE_AREA2, display.width(), display.height(), SSD1306_BLACK);
    if (refreshDisplay) {
      display.display(); 
    }
  }
}



void displayLocalShiftState(){
  String msg;

  switch (localShiftMode) {                      
    case NO_LOCAL_SHIFT:
      msg = MSG_NORMAL_MODE;
      break;
    case LOCAL_SHIFT_TEMP:
      msg = "Temp.SHIFT";
      break;
    case LOCAL_SHIFT_LOCKED:
      msg = "SHIFT";
      break;
  }
  
  if (displayPresent) {
    clearAreaUPDisplay();
    display.setCursor(0, 0);
    display.print(msg);
    display.display(); 
  }
  
  Serial.println("Shift state: " + msg);
}



void displayKeyLabel(String labels[ROWS * COLS], int keyIndex){
  if (displayPresent) {  
    clearAreaBottomDisplay();
    display.setCursor(0, INIT_LINE_AREA2);
    display.print(labels[keyIndex]);
    display.display(); 
  }
  
  Serial.println(labels[keyIndex]);
}


void displayTestMode() {
  String msg = (testMode) ? "Test mode" : MSG_NORMAL_MODE;
  if (displayPresent) {  
    clearAreaUPDisplay();
    display.setCursor(0, 0);
    display.print(msg);
    display.display(); 
  }
  
  Serial.println((testMode) ? "Test ON" : "Test Off");
}
