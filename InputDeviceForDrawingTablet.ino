/* This project as been made by Jose Ignacio Alonso
 * For more information, visit https://github.com/Alonsog2/InputDeviceForDrawingTablet 
 */


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

byte rowPins[ROWS] = {7, 8, 9, 10}; //rows 
byte colPins[COLS] = {3, 4, 5, 6};  //columns

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char actions[ROWS * COLS][2] = {  // code sent when a key is pressed. if second value is other than 0 send it
  {'q', 0},            {'t', 0},             {KEY_LEFT_CTRL, 'd'},{'|', 0},
  {KEY_TAB, 0},        {'s', 0},             {'r', 0},            {KEY_BACKSPACE, 0},
  {KEY_LEFT_SHIFT, 0}, {'N', 0},             {'z', 0},            {'v', 0},
  {KEY_LEFT_CTRL, 0},  {KEY_LEFT_CTRL, 'z'}, {KEY_LEFT_ALT, 0},   {' ',0}
};

// Next actions are active when localShiftKey has been pressed, insted of the 'actions' array. 
char actions_LocalShift[ROWS * COLS][2] = {  // code sent when a key is pressed. if second value is other than 0 send it
  {'m',0},              {KEY_LEFT_CTRL, 't'}, {KEY_F3, 0},          {'|',0},
  {KEY_LEFT_CTRL, 'a'}, {KEY_LEFT_CTRL, 'g'}, {KEY_LEFT_ALT, 'g'},  {0xB0, 'b'},
  {'x', 0},             {KEY_LEFT_CTRL, 'b'}, {KEY_LEFT_CTRL, 'k'}, {KEY_LEFT_CTRL, 'l'},
  {KEY_LEFT_CTRL, 'x'}, {KEY_LEFT_CTRL, 'c'}, {KEY_LEFT_CTRL, 'v'}, {KEY_LEFT_SHIFT, 'v'}
};

int index_LocalShiftKey = 3;                // index from 0 to 15 of the key that acts a 'shift' key. If -1 then no localShiftKey 

enum LOCAL_SHIFT_MODES {
  NO_LOCAL_SHIFT = 0,                        
  LOCAL_SHIFT_TEMP,                         // Pressing once localShift key. Then, after pressing any other key, the system automaticaly return to a NO_LOCAL_SHIFT mode 
  LOCAL_SHIFT_LOCKED,                       // Pressing twice localShift key. Then the system is in localShift mode until localShift key is pressed again.
};

char key;
LOCAL_SHIFT_MODES localShiftMode = NO_LOCAL_SHIFT;

#define pin_LED_LocalShift A2
#define millis_LED_LocalShift 100
long lastMillis_LED_LocalShift = 0;


//////////////////////////////////////////////////////////////////////  ROTARY ENCODERS  ///////////////////////////////////////////////////////////////

#include <Encoder.h>

#define N_ENCODERS 3

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc0(2,16);
Encoder myEnc1(0,14);
Encoder myEnc2(1,15);
//   avoid using pins with LEDs attached

#define INX_ENCODER_UP 0
#define INX_ENCODER_DOWN 1

char actions1Encoders[N_ENCODERS][2] = {       // Codes sent by the encoders (index [n][INX_ENCODER_UP]=UP or index [n][INX_ENCODER_DOWN]=DOWN)
  {'4',  '6'},                                 // Rotation
  {222,  223},                                 // 223=+ 222=-  Zoom
  {'7',  '9'}                                  // Brush size (not standard krita shortcut. Must be assigned in krita too)
};

char actions2Encoders[N_ENCODERS][2] = {       // second code to send, if needed (the value is other than 0)
  {0,  0},
  {0,  0}, 
  {0,  0}
};



//////////////////////////////////////////////////////////////////////   Analog button (3 keys readed in only one analogic input)  ////////////////////////

#include <AnalogMultiButton.h>

// define the pin you want to use
const int ENCODER_BUTTONS_ANALOG_PIN = A3;

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

// make an AnalogMultiButton object, pass in the pin, total and values array
AnalogMultiButton encoder_buttons(ENCODER_BUTTONS_ANALOG_PIN, ENCODER_BUTTONS_TOTAL, ENCODER_BUTTONS_VALUES);

char actionsEncoder_Buttons[N_ENCODERS][2] = {       // First key sent for each encoder_button. if other than 0, second key sent for each encoder_button
  {'5', 0},   // Reset rotation
  {'2', 0},   // Reset zoom (fit to page)
  {'e', 0}    // Toggle eraser mode
};



/////////////////////////////////////////////////////// setup //////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  //Switch off the serial port leds
  pinMode(LED_BUILTIN_TX, INPUT);
  pinMode(LED_BUILTIN_RX, INPUT);

  pinMode(pin_LED_LocalShift, OUTPUT);                       // LED for localShift indication
  refreshLEDLocalShift();
  
  keypad.setDebounceTime(50);

  Keyboard.begin();
}




/////////////////////////////////////////////////////// loop //////////////////////////////////////////////////////////////

void loop() {
  String msg;
  int keyIndex;
  // Fills kpd.key[ ] array with up-to 10 active keys. Returns true if there are ANY active keys.
  if (keypad.getKeys()) {
    for (int i=0; i<LIST_MAX; i++) {                            // Scan the whole key list.
      if ( keypad.key[i].stateChanged ) {                       // Only find keys that have changed state.
          keyIndex = keypad.key[i].kchar - char('0');           // get the index from 0 to 15
          switch (keypad.key[i].kstate) {                       // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
            case PRESSED:
              msg = " PRESSED.";

              if (keyIndex == index_LocalShiftKey) {
                switch (localShiftMode){
                  case NO_LOCAL_SHIFT:
                    localShiftMode = LOCAL_SHIFT_TEMP;
                    break;
                  case LOCAL_SHIFT_TEMP:
                    localShiftMode = LOCAL_SHIFT_LOCKED;
                    lastMillis_LED_LocalShift = 0;
                    break;
                  case LOCAL_SHIFT_LOCKED:
                    localShiftMode = NO_LOCAL_SHIFT;
                    break;
                }
                Keyboard.releaseAll();                         // just in case some other previous key still pressed...
                refreshLEDLocalShift();
                break;
              }
              
              sendKeyPressed( ( (localShiftMode == NO_LOCAL_SHIFT) ? actions : actions_LocalShift), keyIndex);
              break;
              
            case HOLD:
              msg = " HOLD.";
              break;
              
            case RELEASED:
              msg = " RELEASED.";

              if (keyIndex == index_LocalShiftKey) {
                Keyboard.releaseAll();                        // just in case some other previous key still pressed...
                break;
              }
              if (localShiftMode == LOCAL_SHIFT_TEMP) {
                localShiftMode = NO_LOCAL_SHIFT;
                Keyboard.releaseAll();
                refreshLEDLocalShift();
              }
              
              sendKeyDepressed( ( (localShiftMode) ? actions_LocalShift : actions), keyIndex);        
              break;
              
            case IDLE:
              msg = " IDLE.";
              break;
          }
          //Serial.print("Key ");
          //Serial.print(keypad.key[i].kchar);
          //Serial.println(msg);
      }
    }
  }
  refreshLEDLocalShift();
  
  
  // check the encoders 
  long newPositionEncoder;              

  for (int nEncoder=0; nEncoder < N_ENCODERS ; nEncoder++) {
    int indexUpDown = -1;
    
    newPositionEncoder = chekEncoderN(nEncoder);
    if (newPositionEncoder > 1) indexUpDown=INX_ENCODER_UP;
    if (newPositionEncoder < -1) indexUpDown=INX_ENCODER_DOWN;
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
  
  if(encoder_buttons.onPressAndAfter(ENCODER0_BUTTON, 0)) {    // 0= Detect only the first pressing
    Keyboard.press(actionsEncoder_Buttons[0][0]);
    if (actionsEncoder_Buttons[0][1] != 0) {                     // if the value is other than 0, send it
      Keyboard.press(actionsEncoder_Buttons[0][1]);
    }
  }
  
  if(encoder_buttons.onPressAndAfter(ENCODER1_BUTTON, 0)) {    
    Serial.println("btn1");
    Keyboard.press(actionsEncoder_Buttons[1][0]);
    if (actionsEncoder_Buttons[1][1] != 1) {                    
      Keyboard.press(actionsEncoder_Buttons[1][1]);
    }
  }
  
  if(encoder_buttons.onPressAndAfter(ENCODER2_BUTTON, 0)) {     
    Serial.println("btn2");
    Keyboard.press(actionsEncoder_Buttons[2][0]);
    if (actionsEncoder_Buttons[2][1] != 0) {                    
      Keyboard.press(actionsEncoder_Buttons[2][1]);
    }
 
  }
  
  // check if some encoder_button has been depressed
  if(encoder_buttons.onRelease(ENCODER0_BUTTON) || encoder_buttons.onRelease(ENCODER1_BUTTON) || encoder_buttons.onRelease(ENCODER2_BUTTON)) {
    Keyboard.releaseAll();
  }
  //

  
}
    



/////////////////////////////////////////////////////// other functions //////////////////////////////////////////////////////////////

void sendKeyPressed(char acts[ROWS * COLS][2], int keyIndex){
   Keyboard.press(acts[keyIndex][0]);           // send the first action for the corresponding key
  if (acts[keyIndex][1] != 0) {                 // send the second value if not 0
    Keyboard.press(acts[keyIndex][1]);
  }
}



void sendKeyDepressed(char acts[ROWS * COLS][2], int keyIndex){
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



void refreshLEDLocalShift(){
  static boolean prevStatusLedOn = false;
  boolean newStatusLed;
  
  if (localShiftMode == LOCAL_SHIFT_TEMP) {                                // if LOCAL_SHIFT_TEMP mode, blink led
    newStatusLed = prevStatusLedOn;
    if (millis() > (lastMillis_LED_LocalShift + millis_LED_LocalShift) ) {
      lastMillis_LED_LocalShift = millis();
      newStatusLed = (! newStatusLed);
    }
  } else {
    newStatusLed = ( (localShiftMode == NO_LOCAL_SHIFT) ? LOW : HIGH);     // other modes than LOCAL_SHIFT_TEMP
  }

  digitalWrite(pin_LED_LocalShift, newStatusLed);
  prevStatusLedOn = newStatusLed;
}
