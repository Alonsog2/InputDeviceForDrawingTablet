/* This project as been made by Jose Ignacio Alonso
   For more information, visit https://github.com/Alonsog2/InputDeviceForDrawingTablet
*/


#define SETUP2USE "Setup2.h"



///////////////////////////////////////////////////////////////////////////////   4X4 keyboard   ///////////////////////////////////////////////////////

#include <Keyboard.h>               // library for USB communication

#include <Keypad.h>                 // library for controlling 4x4 keys
const byte ROWS = 4; // rows
const byte COLS = 4; // columns
char keys[ROWS][COLS] = {           // characters behind number 9 are the next in the ASCII secuence, just to make easier the indexation (from 0 to 15)
  {'0', '1', '2', '3'},
  {'4', '5', '6', '7'},
  {'8', '9', ':', ';'},
  {'<', '=', '>', '?'}
};

enum LOCAL_SHIFT_MODES {
  NO_LOCAL_SHIFT = 0,
  LOCAL_SHIFT_TEMP,                         // Pressing once localShift key. Then, after pressing any other key, the system automaticaly return to a NO_LOCAL_SHIFT mode
  LOCAL_SHIFT_LOCKED,                       // Pressing twice localShift key. Then the system is in localShift mode until localShift key is pressed again.
};

char key;
LOCAL_SHIFT_MODES localShiftMode = NO_LOCAL_SHIFT;

#define millis_LED_LocalShift 150
#define millis_LED_TestMode_High 50
#define millis_LED_TestMode_Low 900
long lastMillis_LED_Status = 0;


//////////////////////////////////////////////////////////////////////  ROTARY ENCODERS  ///////////////////////////////////////////////////////////////

#include <Encoder.h>

#define N_ENCODERS 3

#define INX_ENCODER_UP 0
#define INX_ENCODER_DOWN 1


//////////////////////////////////////////////////////////////////////   Analog button (3 keys readed in only one analogic input)  ////////////////////////

#include <AnalogMultiButton.h>

// set how many buttons you have connected
const int ENCODER_BUTTONS_TOTAL = N_ENCODERS;

// find out what the value of analogRead is when you press each of your buttons and put them in this array
// you can find this out by putting Serial.println(analogRead(ENCODER_BUTTONS_ANALOG_PIN)); in your loop() and opening the serial monitor to see the values
// make sure they are in order of smallest to largest
const int ENCODER_BUTTONS_VALUES[ENCODER_BUTTONS_TOTAL] = {0, 315, 482};

// you can also define constants for each of your buttons, which makes your code easier to read
// define these in the same order as the numbers in your BUTTONS_VALUES array, so whichever button has the smallest analogRead() number should come first
const int ENCODER0_BUTTON = 0;
const int ENCODER1_BUTTON = 1;
const int ENCODER2_BUTTON = 2;


#include SETUP2USE    
#include "Display.h"
#include "MIDI.h"


#define CF(s) ((const __FlashStringHelper *)s)

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// make an AnalogMultiButton object, pass in the pin, total and values array
AnalogMultiButton encoder_buttons(ENCODER_BUTTONS_ANALOG_PIN, ENCODER_BUTTONS_TOTAL, ENCODER_BUTTONS_VALUES);

boolean testMode = false;


/////////////////////////////////////////////////////// setup //////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println(F("Starting..."));

  if (bUseDisplay) {
    initDisplay();
  }

  //Switch off the serial port leds
  pinMode(LED_BUILTIN_TX, INPUT);
  pinMode(LED_BUILTIN_RX, INPUT);

  pinMode(pin_LED_Status, OUTPUT);                       // LED for localShift indication
  
  if (bUseLEDTestMode) { 
    pinMode(pin_LED_TestMode, OUTPUT);                   // LED for TestMode indication (If enabled in setup)
  }

  refreshLEDs_Status();

  keypad.setDebounceTime(50);

  Keyboard.begin();

  Serial.println(F("FinSetup"));
}




/////////////////////////////////////////////////////// loop //////////////////////////////////////////////////////////////

void loop() {
  int keyIndex;
  // Fills kpd.key[ ] array with up-to 10 active keys. Returns true if there are ANY active keys.
  if (keypad.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++) {                       // Scan the whole key list.
      if ( keypad.key[i].stateChanged ) {                      // Only find keys that have changed state.
        keyIndex = keypad.key[i].kchar - char('0');            // get the index from 0 to 15
        switch (keypad.key[i].kstate) {                        // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
            if (MIDImode) {
              pressedMIDIKey(keypad.key[i].kchar);
              break;  
            }
            
            if (keyIndex == index_LocalShiftKey) {
              switch (localShiftMode) {
                case NO_LOCAL_SHIFT:
                  localShiftMode = (testMode) ? LOCAL_SHIFT_LOCKED : LOCAL_SHIFT_TEMP;  // In testMode, always go to LOCAL_SHIFT_LOCKED (skiping LOCAL_SHIFT_TEMP) 
                  break;
                case LOCAL_SHIFT_TEMP:
                  localShiftMode = LOCAL_SHIFT_LOCKED;
                  lastMillis_LED_Status = 0;
                  break;
                case LOCAL_SHIFT_LOCKED:
                  localShiftMode = NO_LOCAL_SHIFT;
                  break;
              }
              Keyboard.releaseAll();                         // just in case some other previous key still pressed...
              displayStatus();
              //refreshLEDs_Status();
              break;
            }

            if (! testMode) {
              sendKeyPressed( ( (localShiftMode == NO_LOCAL_SHIFT) ? actions : actions_LocalShift), keyIndex);
            }

            if (localShiftMode == NO_LOCAL_SHIFT) {
              displayKeyLabel( CF( actions_labels[keyIndex]));
            } else {
              displayKeyLabel( CF( actions_LocalShift_labels[keyIndex]));
            }
            break;

          case HOLD:
            if (testModeEnabled && (keyIndex == index_LocalShiftKey)) {  // Switch test mode
              Keyboard.releaseAll();                                     // just in case some other previous key still pressed...
              localShiftMode = NO_LOCAL_SHIFT;                           // After enter or exit testMode, always return to NO_LOCAL_SHIFT modee2
              testMode= !testMode;
              displayStatus();
              //refreshLEDs_Status();
            }
            break;

          case RELEASED:
            if (MIDImode) {
              releasedMIDIKey(keypad.key[i].kchar);
              break;  
            }
            
            if (keyIndex == index_LocalShiftKey) {
              Keyboard.releaseAll();                        // just in case some other previous key still pressed...
              break;
            }
            if (localShiftMode == LOCAL_SHIFT_TEMP) {
              localShiftMode = NO_LOCAL_SHIFT;
              Keyboard.releaseAll();
              displayStatus();
              refreshLEDs_Status();
            }

            sendKeyDepressed( ( (localShiftMode) ? actions_LocalShift : actions), keyIndex);
            clearAreaBottomDisplay(true);
            break;

            //case IDLE:
            //  msg = " IDLE.";
            //  break;
        }

        //Serial.print(F("Key "));
        //Serial.print(keypad.key[i].kchar);
      }
    }
  }
  refreshLEDs_Status();


  // check the encoders
  long newPositionEncoder;

  for (int nEncoder = 0; nEncoder < N_ENCODERS ; nEncoder++) {
    int indexUpDown = -1;

    newPositionEncoder = chekEncoderN(nEncoder);
    if (newPositionEncoder > 1) indexUpDown = INX_ENCODER_UP;
    if (newPositionEncoder < -1) indexUpDown = INX_ENCODER_DOWN;
    if (indexUpDown != -1) {
      Keyboard.press(actions1Encoders[nEncoder][indexUpDown]);
      if (actions2Encoders[nEncoder][indexUpDown] != 0) {                    // if the value is other than 0, send it
        Keyboard.press(actions2Encoders[nEncoder][indexUpDown] );
      }
      resetEncoderN(nEncoder);
      Keyboard.releaseAll();
    }
  }
  //


  // check encoder-buttons  ////////////////////////////////////////////////////////////////////////////////////////
  encoder_buttons.update();

  if (encoder_buttons.onPressAndAfter(ENCODER0_BUTTON, 0)) {     // 0= Detect only the first pressing
    Keyboard.press(actionsEncoder_Buttons[0][0]);
    if (actionsEncoder_Buttons[0][1] != 0) {                     // if the value is other than 0, send it
      Keyboard.press(actionsEncoder_Buttons[0][1]);
    }
  }

  if (encoder_buttons.onPressAndAfter(ENCODER1_BUTTON, 0)) {
    Keyboard.press(actionsEncoder_Buttons[1][0]);
    if (actionsEncoder_Buttons[1][1] != 1) {
      Keyboard.press(actionsEncoder_Buttons[1][1]);
    }
  }

  if (encoder_buttons.onPressAndAfter(ENCODER2_BUTTON, 0)) {
    MIDImode = !MIDImode;
    switchMIDIAndKeyboard();
//    Keyboard.press(actionsEncoder_Buttons[2][0]);
//    if (actionsEncoder_Buttons[2][1] != 0) {
//      Keyboard.press(actionsEncoder_Buttons[2][1]);
//    }

  }

  // check if some encoder_button has been depressed
  if (encoder_buttons.onRelease(ENCODER0_BUTTON) || encoder_buttons.onRelease(ENCODER1_BUTTON) || encoder_buttons.onRelease(ENCODER2_BUTTON)) {
    Keyboard.releaseAll();
  }
  //

} // loop




///////////////////////////////////////////////////////// other functions //////////////////////////////////////////////////////////////

void sendKeyPressed(char acts[ROWS * COLS][2], int keyIndex) {
  Keyboard.press(acts[keyIndex][0]);           // send the first action for the corresponding key
  if (acts[keyIndex][1] != 0) {                 // send the second value if not 0
    Keyboard.press(acts[keyIndex][1]);
  }
}



void sendKeyDepressed(char acts[ROWS * COLS][2], int keyIndex) {
  if (acts[keyIndex][1] != 0) {                  // invert the order when releasing the keys, send actions2 first (if needed)
    Keyboard.release(acts[keyIndex][1]);
  }
  Keyboard.release(acts[keyIndex][0]);           // and then actions1
}



long chekEncoderN(int nEncoder) {
  switch (nEncoder) {
    case 0:
      return myEnc0.read();
      break;
    case 1:
      return myEnc1.read();
      break;
    case 2:
      return myEnc2.read();
      break;
      return 0;
  }
}



void resetEncoderN(int nEncoder) {
  switch (nEncoder) {
    case 0:
      myEnc0.write(0);
      break;
    case 1:
      myEnc1.write(0);
      break;
    case 2:
      myEnc2.write(0);
      break;
  }
}



void refreshLEDs_Status() {
  static boolean prevStatusLed = false;
  static boolean prevStatusLed_TestMode = false;
  static int nPulseTest = 0; 
  boolean newStatusLed;
  boolean newStatusLed_TestMode;

  if (testMode) {                                                                 // TestMode 
    if (bUseLEDTestMode) {                                                        // Using 2 LED for status (Shift and TestMode)
      if (localShiftMode == NO_LOCAL_SHIFT) {
        newStatusLed_TestMode = HIGH;
      } else {
        newStatusLed_TestMode = prevStatusLed_TestMode;
        if (millis() > (lastMillis_LED_Status + millis_LED_LocalShift) ) {
          lastMillis_LED_Status = millis();
          newStatusLed_TestMode = (! prevStatusLed_TestMode);
        }
      }
      digitalWrite(pin_LED_TestMode, newStatusLed_TestMode);
      prevStatusLed_TestMode = newStatusLed_TestMode;
      newStatusLed = LOW;

  
    } else {                                                                      // Using 1 LED for status (Shift and TestMode)
      newStatusLed = prevStatusLed;
      switch (nPulseTest) {                                                       
        case 0:                                                                   // NO_LOCAL_SHIFT status -> One short blink and a large pause
          if (millis() > (lastMillis_LED_Status + millis_LED_TestMode_High) ) {
            lastMillis_LED_Status = millis();
            newStatusLed = false;  
            nPulseTest ++;
          }
          break;
        case 1:                                                       
          if (millis() > (lastMillis_LED_Status + millis_LED_TestMode_High) ) {
            lastMillis_LED_Status = millis();
            if (localShiftMode != NO_LOCAL_SHIFT) {                               // Other status than NO_LOCAL_SHIFT -> Two short blink and a large pause
              newStatusLed = true;
            }
            nPulseTest ++;  
          }
          break;         
        case 2:
          if (millis() > (lastMillis_LED_Status + millis_LED_TestMode_High) ) {
            lastMillis_LED_Status = millis();
            newStatusLed = false;  
            nPulseTest ++;
          }
          break;   
        case 3:
          if (millis() > (lastMillis_LED_Status + millis_LED_TestMode_Low) ) {
            lastMillis_LED_Status = millis();
            newStatusLed = true; 
            nPulseTest = 0; 
          }
          break;       
      } 
    }

  } else {                                                                   // NO testMode (normal)
    if (bUseLEDTestMode) { 
      digitalWrite(pin_LED_TestMode, LOW);                                   // LedTestMode Off
    }
    if (localShiftMode == LOCAL_SHIFT_TEMP) {                                // if LOCAL_SHIFT_TEMP mode, blink led
      newStatusLed = prevStatusLed;
      if (millis() > (lastMillis_LED_Status + millis_LED_LocalShift) ) {
        lastMillis_LED_Status = millis();
        newStatusLed = (! newStatusLed);
      }
    } else {
      newStatusLed = ( (localShiftMode == NO_LOCAL_SHIFT) ? LOW : HIGH);     // other modes than LOCAL_SHIFT_TEMP
    }
  }

  digitalWrite(pin_LED_Status, newStatusLed);
  prevStatusLed = newStatusLed;
}



void switchMIDIAndKeyboard(){
  if (MIDImode){
    digitalWrite(pin_LED_Status, HIGH);      // both LEDS ON;
    digitalWrite(pin_LED_TestMode, HIGH);
  } else {
    localShiftMode == NO_LOCAL_SHIFT;   // refresh LED status inside 'loop'
  }
}
