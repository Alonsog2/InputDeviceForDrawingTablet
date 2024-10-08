/* This project as been made by Jose Ignacio Alonso
   For more information, visit https://github.com/Alonsog2/InputDeviceForDrawingTablet
*/


#define SETUP_TO_USE "Setup1.h"

//#define INIT_IN_ TEST_MODE           // uncomment this line while testing the sketch, to prevent sending keystrokes to the computer


#define CF(s) ((const __FlashStringHelper *)s)

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
  LOCAL_SHIFT_TEMP,                         // Pressing once localShift key. Then, after pressing any other key, the system automaticaly return to NO_LOCAL_SHIFT mode
  LOCAL_SHIFT_LOCKED,                       // Pressing twice localShift key. Then the system remains in localShift mode until localShift key is pressed again.
};

LOCAL_SHIFT_MODES localShiftMode = NO_LOCAL_SHIFT;

#define millis_LED_LocalShift 150
#define millis_LED_TestMode_High 50
#define millis_LED_TestMode_Low 900
#define millis_LED_MidiMode 1000
#define millis_LONG_PRESS_SwitchMIDImode 2000

long lastMillis_LED_Status = 0;

char key;

//////////////////////////////////////////////////////////////////////  ROTARY ENCODERS  ///////////////////////////////////////////////////////////////

#include <Encoder.h>

#define N_ENCODERS 3

#define INX_ENCODER_UP 1
#define INX_ENCODER_DOWN 0

#define MILLIS_DISPLAYING_ENCODER_INFO 600
boolean bEncodersMoved = false;
unsigned long lastTimeEncoderMoved = 0;


//////////////////////////////////////////////////////////////////////   Analog buttons (3 keys readed in only one analogic input)  ////////////////////////

#include <AnalogMultiButton.h>

// set how many buttons you have connected
#define ENCODER_BUTTONS_TOTAL N_ENCODERS

// find out what the value of analogRead is when you press each of your buttons and put them in this array
// you can find this out by putting Serial.println(analogRead(ENCODER_BUTTONS_ANALOG_PIN)); in your loop() and opening the serial monitor to see the values
// make sure they are in order of smallest to largest
const int ENCODER_BUTTONS_VALUES[ENCODER_BUTTONS_TOTAL] = {0, 315, 482};

// you can also define constants for each of your buttons, which makes your code easier to read
// define these in the same order as the numbers in your BUTTONS_VALUES array, so whichever button has the smallest analogRead() number should come first
enum ENCODER_BUTTONS {
  ENCODER2_BUTTON = 0,
  ENCODER1_BUTTON,
  ENCODER0_BUTTON
};


#include SETUP_TO_USE

#include "MIDI.h"


Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// make an AnalogMultiButton object, pass in the pin, total and values array
AnalogMultiButton encoder_buttons(ENCODER_BUTTONS_ANALOG_PIN, ENCODER_BUTTONS_TOTAL, ENCODER_BUTTONS_VALUES);

#ifdef INIT_IN_ TEST_MODE
  boolean testMode = true;
#else
  boolean testMode = false;
#endif


#define SECONDS_UNTIL_SCREENSAVER_IF_SYSTEMUNLOCKED 300   // (300/60) = 5 minutes
#define SECONDS_UNTIL_SCREENSAVER_IF_SYSTEMLOCKED 5     

unsigned long lastTimeActionByUser = 0;
unsigned long millisIntervalUntilScreenSaverIfSystemUnlocked = (unsigned long)(SECONDS_UNTIL_SCREENSAVER_IF_SYSTEMUNLOCKED) * 1000;
unsigned long millisIntervalUntilScreenSaverIfSystemLocked = (unsigned long)(SECONDS_UNTIL_SCREENSAVER_IF_SYSTEMLOCKED) * 1000;
unsigned long millisIntervalScreenSaver = millisIntervalUntilScreenSaverIfSystemUnlocked;
boolean bScreenSaverON = false;
boolean bSystemLocked = false;



/////////////////////////////////////////////////////// setup //////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  delay(3000);
  Serial.println(F("Init"));

  if (DeviceModel != KEYBOARD_ONLY) {
    initMIDI();
  }

  if (bUseDisplay) {
    initDisplay();
  }

  //Switch off the serial port leds
  pinMode(LED_BUILTIN_TX, INPUT);
  pinMode(LED_BUILTIN_RX, INPUT);

  pinMode(pin_LED_Status, OUTPUT);                       // LED for localShift indication

  refreshLED_Status();

  keypad.setDebounceTime(50);

  Keyboard.begin();

  encoder_buttons.update();                              // just for clearing status

  Serial.println(F("EndSetup"));
}




/////////////////////////////////////////////////////// loop //////////////////////////////////////////////////////////////

void loop() {
  byte keyIndex;

  encoder_buttons.update();                                                                  // check encoder_buttons only once for the whole loop
  
  if (bCheckMIDIpedalIn4x4keysMode){                                                         // Checking MIDIpedal while in 4x4keys mode 
    if (encoder_buttons.onPressAndAfter(ENCODER2_BUTTON, 0)) {                               // 0= Detect only the first pressing
       pressedMIDIKey(actionsMIDIEncodersButtons, 2);                                        // MIDIpedal is button #2       
    }

    if (encoder_buttons.onRelease(ENCODER2_BUTTON)) {
      releasedMIDIKey(actionsMIDIEncodersButtons, 2);
      clearAreaBottomDisplay();
    }
  }

  unsigned long millisNow = millis();
  if ( abs(millisNow - lastTimeActionByUser) > millisIntervalScreenSaver ) {
    if (! bScreenSaverON) {
      clearAllDisplay(); 
      bScreenSaverON = true;
    }
    bSystemLocked = true;
  }

  if (bSystemLocked) {
    if (keypad.getKeys()) {
      for (int i = 0; i < LIST_MAX; i++) {                       // Scan the whole key list.
        if ( keypad.key[i].stateChanged ) {                      // Only find keys that have changed state.
          keyIndex = keypad.key[i].kchar - char('0');            // get the index from 0 to 15
          if (keyIndex == index_LocalShiftKey) {
            if (keypad.key[i].kstate == HOLD) {
              bSystemLocked = false;
            }
          }
        }
      }
      displayStatus();
      resetTimeScreenSaver();      
    }
    if (bSystemLocked) {
      millisIntervalScreenSaver = millisIntervalUntilScreenSaverIfSystemLocked;
      return;
    } else {
      millisIntervalScreenSaver = millisIntervalUntilScreenSaverIfSystemUnlocked;
    }
  }

  // Fills kpd.key[ ] array with up-to 10 active keys. Returns true if there are ANY active keys.
  if (keypad.getKeys()) {
    for (int i = 0; i < LIST_MAX; i++) {                       // Scan the whole key list.
      if ( keypad.key[i].stateChanged ) {                      // Only find keys that have changed state.
        resetTimeScreenSaver();
        bEncodersMoved = false;
        keyIndex = keypad.key[i].kchar - char('0');            // get the index from 0 to 15
        switch (keypad.key[i].kstate) {                        // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
          case PRESSED:
            if (MIDImode) {
              pressedMIDIKey(actionsMIDI, keyIndex);
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
              clearAllDisplay();
              displayStatus();
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
              testMode = !testMode;
              displayStatus();
            }
            break;

          case RELEASED:
            if (MIDImode) {
              releasedMIDIKey(actionsMIDI, keyIndex);
              clearAreaBottomDisplay();
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
              refreshLED_Status();
            }

            sendKeyDepressed( ( (localShiftMode) ? actions_LocalShift : actions), keyIndex);
            clearAreaBottomDisplay();
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
  refreshLED_Status();

  // check the ENCODERS
  if (bUseRotaryEncoders) {
    long newPositionEncoder;

    for (int nEncoder = 0; nEncoder < N_ENCODERS ; nEncoder++) {
      int indexUpDown = -1;

      newPositionEncoder = chekEncoderN(nEncoder);
      if (newPositionEncoder > 1) indexUpDown = INX_ENCODER_DOWN;
      if (newPositionEncoder < -1) indexUpDown = INX_ENCODER_UP;
      if (indexUpDown != -1) {
        if (MIDImode) { /////////////////////////////////////////////////////////////////// MIDI mode /////////////////////////////////////////
          if (MIDIencodersEqualUpDownCC[nEncoder]) { // MIDI encoder EqualUpDownCC
            byte channel = (actionsMIDIEncoders[nEncoder][MIDI_CHANNEL] == 0) ? GLOBAL_MIDI_CHANNEL : actionsMIDIEncoders[nEncoder][MIDI_CHANNEL];
            int value = MIDIvalRotaryEncoders[nEncoder];
            if (indexUpDown == INX_ENCODER_UP) {
              value += actionsMIDIEncoders[nEncoder][MIDI_IncrDecrStep];
              if (value > actionsMIDIEncoders[nEncoder][MIDI_ValMax]) {
                value = actionsMIDIEncoders[nEncoder][MIDI_ValMax];
              }
            }
            if (indexUpDown == INX_ENCODER_DOWN) {
              value -= actionsMIDIEncoders[nEncoder][MIDI_IncrDecrStep];
              if (value < actionsMIDIEncoders[nEncoder][MIDI_ValMin]) {
                value = actionsMIDIEncoders[nEncoder][MIDI_ValMin];
              }
            }

            MIDIvalRotaryEncoders[nEncoder] = value;
            sendCtrlChange_USB(actionsMIDIEncoders[nEncoder][MIDI_nCC], value, channel);
          } else {  // Dual UP/DOWN
            pressedMIDIKey( ((indexUpDown == INX_ENCODER_DOWN) ? actionsMIDIEncodersDOWN : actionsMIDIEncodersUP ),  nEncoder);
          }
          // End MIDI

        } else { ///////////////////////////////////////////////////////////////////// KEYBOARD mode /////////////////////////////////////////
          if (! testMode) {
            Keyboard.press(actions1Encoders[nEncoder][indexUpDown]);
            if (actions2Encoders[nEncoder][indexUpDown] != 0) {                    // if the value is other than 0, send it
              Keyboard.press(actions2Encoders[nEncoder][indexUpDown] );
            }
          }

          displayKeyLabel( (indexUpDown == INX_ENCODER_UP) ? CF(actionsEncoderIncr_labels[nEncoder]) : CF(actionsEncoderDecr_labels[nEncoder]));

          Keyboard.releaseAll();
          // End Keyboard
        }

        resetEncoderN(nEncoder);
        resetTimeScreenSaver();
        lastTimeEncoderMoved = millis();
        bEncodersMoved = true;
      }
    }
  }
  //


  // check ENCODER_BUTTONS  ////////////////////////////////////////////////////////////////////////////////////////
  if (bUseEncoderButtons) {
    int keyIndex = -1;
    //encoder_buttons has been updated at the begining of the loop

    // Check possible switch Keyboard/MIDI mode //////////////////////////////////
    if (DeviceModel == KEYBOARD_AND_MIDI || DeviceModel == MIDI_AND_KEYBOARD) {              // Device can switch between Keyboard and MIDI ?
      if (encoder_buttons.onPressAfter(ENCODER0_BUTTON, millis_LONG_PRESS_SwitchMIDImode)) { // The defined enconder for switching is pressed more than xxxx millis ?
        Keyboard.releaseAll();
        resetTimeScreenSaver();
        bEncodersMoved = false;
        switchMIDIAndKeyboardMode();
      }
    } ////////////////////////////////////////////////////////////////////////////

    if (encoder_buttons.onPressAndAfter(ENCODER0_BUTTON, 0)) {   // 0= Detect only the first pressing
      keyIndex = 0;
    } else if (encoder_buttons.onPressAndAfter(ENCODER1_BUTTON, 0)) {
      keyIndex = 1;
    } else if (encoder_buttons.onPressAndAfter(ENCODER2_BUTTON, 0) && (!bCheckMIDIpedalIn4x4keysMode)) {
      keyIndex = 2;
    }

    if (keyIndex != -1) {
      resetTimeScreenSaver();
      bEncodersMoved = false;
      if (MIDImode) {
        pressedMIDIKey(actionsMIDIEncodersButtons, keyIndex);
      } else {
        if (! testMode) {
          sendKeyPressed(actionsEncoder_Buttons, keyIndex);
        }
        displayKeyLabel( CF( actionsEncoder_labels[keyIndex]));
      }
    }

    // check if some encoder_button has been DEPRESSED
    keyIndex = -1;
    if (encoder_buttons.onRelease(ENCODER0_BUTTON)) {
      keyIndex = 0;
    } else if (encoder_buttons.onRelease(ENCODER1_BUTTON)) {
      keyIndex = 1;
    } else if (encoder_buttons.onRelease(ENCODER2_BUTTON) && (!bCheckMIDIpedalIn4x4keysMode)) {
      keyIndex = 2;
    }

    if (keyIndex != -1) {
      resetTimeScreenSaver();
      bEncodersMoved = false;
      if (MIDImode) {                                            // MIDI mode
        releasedMIDIKey(actionsMIDIEncodersButtons, keyIndex);
      } else {                                                   // Keyboard
        if (! testMode) {
          sendKeyDepressed(actionsEncoder_Buttons, keyIndex);
        }
      }
      clearAreaBottomDisplay();
    }

  } // End ENCODER_BUTTONS

  checklastTimeEncoderMoved();

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



void refreshLED_Status() {
  static boolean prevStatusLed = false;
  static boolean prevStatusLed_TestMode = false;
  static byte nPulseTest = 0;
  boolean newStatusLed;
  boolean newStatusLed_TestMode;

  if (MIDImode) {                                                                 // MIDI
    refreshLED_inMIDImode();
    return;
  }

  if (testMode) {                                                                 // TestMode
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

  } else {                                                                   // NO testMode (normal)

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



void refreshLED_inMIDImode() {
  static boolean prevStatusLed = false;
  boolean newStatusLed;
  newStatusLed = prevStatusLed;
  if (millis() > (lastMillis_LED_Status + millis_LED_MidiMode) ) {
    lastMillis_LED_Status = millis();
    newStatusLed = (! newStatusLed);
    digitalWrite(pin_LED_Status, newStatusLed);
    prevStatusLed = newStatusLed;
  }
}


void switchMIDIAndKeyboardMode() {
  MIDImode = !MIDImode;
  if (MIDImode) {
    digitalWrite(pin_LED_Status, HIGH);                                      // LED ON;
  } else {
    sendMIDIreset();
    localShiftMode = NO_LOCAL_SHIFT;                                         // will refresh LED status inside 'loop'
    testMode = false;
  }
  displayStatus();
}



void checklastTimeEncoderMoved() {
  if (bEncodersMoved) {
    if ( abs(millis() - lastTimeEncoderMoved) > MILLIS_DISPLAYING_ENCODER_INFO ) {
      clearAreaBottomDisplay();
      bEncodersMoved = false;
    }
  }
}



void resetTimeScreenSaver() {
  bScreenSaverON = false;
  lastTimeActionByUser = millis();
}
