/* This project as been made by Jose Ignacio Alonso
 *  
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

byte rowPins[ROWS] = {7, 8, 9, 10}; //rows (outputs)
byte colPins[COLS] = {3, 4, 5, 6}; //columns (inputs)

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char key;

char actions1[ROWS * COLS] = {               // code sent when a key is pressed
  KEY_DELETE,  'q', 't',  KEY_LEFT_CTRL,
  KEY_TAB, 's',  'r',  KEY_BACKSPACE,
  KEY_LEFT_SHIFT,  'N',  'x',  'v',
  KEY_LEFT_CTRL,  KEY_LEFT_CTRL,  KEY_LEFT_ALT,  ' '
};

char actions2[ROWS * COLS] = {              // second code to send, if needed (the value is other than 0), when a key is pressed. For example KEY_LEFT_CTRL + 'z'
  0,  0,  0, 'd',
  0,  0,  0,  0,
  0,  0,  0,  0,
  0,  'z', 0,  0
};

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

char actions1Encoder_Buttons[N_ENCODERS] = {       // First key send for each encoder_button
  '5',   // Reset rotation
  '2',   // Reset zoom (fit to page)
  'e'    // Toggle eraser mode
};

char actions2Encoder_Buttons[N_ENCODERS] = {       // if other than 0, second key send for each encoder_button
  0,
  0,
  0
};



/////////////////////////////////////////////////////// setup //////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  Serial.println("Startin...");

  //Switch off the serial port leds
  pinMode(LED_BUILTIN_TX, INPUT);
  pinMode(LED_BUILTIN_RX, INPUT);

  keypad.setDebounceTime(50);
  keypad.addEventListener(keypadEvent); // Add an event listener for this keypad

  Keyboard.begin();
}




/////////////////////////////////////////////////////// loop //////////////////////////////////////////////////////////////

void loop() {
  // check the keyboard; if any key has changed, it will be proccesed by the 'keypadEvent' function
  //key = keypad.getKey();                   
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
              Keyboard.press(actions1[keyIndex]);               // send the first action for the corresponding key
              if (actions2[keyIndex] != 0) {                    // send the second value if not 0
                Keyboard.press(actions2[keyIndex]);
              }
              break;
            case HOLD:
              msg = " HOLD.";
              break;
            case RELEASED:
              msg = " RELEASED.";
              //Keyboard.releaseAll();                               
              if (actions2[keyIndex] != 0) {                    // invert the order when releasing the keys, send actions2 first (if needed)
                Keyboard.release(actions2[keyIndex]);           
              }
              Keyboard.release(actions1[keyIndex]);             // and then actions1        
              break;
            case IDLE:
              msg = " IDLE.";
          }
          //Serial.print("Key ");
          //Serial.print(keypad.key[i].kchar);
          //Serial.println(msg);
      }
    }
  }
  
  
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
    Keyboard.press(actions1Encoder_Buttons[0]);
    if (actions2Encoder_Buttons[0] != 0) {                     // if the value is other than 0, send it
      Keyboard.press(actions2Encoder_Buttons[0]);
    }
  }
  
  if(encoder_buttons.onPressAndAfter(ENCODER1_BUTTON, 0)) {    
    Serial.println("btn1");
    Keyboard.press(actions1Encoder_Buttons[1]);
    if (actions2Encoder_Buttons[0] != 1) {                    
      Keyboard.press(actions2Encoder_Buttons[1]);
    }
  }
  
  if(encoder_buttons.onPressAndAfter(ENCODER2_BUTTON, 0)) {     
    Serial.println("btn2");
    Keyboard.press(actions1Encoder_Buttons[2]);
    if (actions2Encoder_Buttons[2] != 0) {                    
      Keyboard.press(actions2Encoder_Buttons[2]);
    }
 
  }
  
  // check if some encoder_button has been depressed
  if(encoder_buttons.onRelease(ENCODER0_BUTTON) || encoder_buttons.onRelease(ENCODER1_BUTTON) || encoder_buttons.onRelease(ENCODER2_BUTTON)) {
    Keyboard.releaseAll();
  }
  //

  
}
    



/////////////////////////////////////////////////////// other functions //////////////////////////////////////////////////////////////

void keypadEvent(KeypadEvent key) {
 return; 
  int keyIndex = key - char('0');   // get the index from 0 to 15)

//  //just for debug
//  String cad = "Key Event " + String(key);
//  Serial.println(cad);
//  Serial.println(keyIndex);
//  //

  switch (keypad.getState()) {
    case PRESSED:
      //Serial.println(String("Accion1 " + String(key)));
      Keyboard.press(actions1[keyIndex]);               // send the first action for the corresponding key
      if (actions2[keyIndex] != 0) {                    // send the second value if is not 0
        //Serial.println(String("Accion2 "  + String(key)));
        Keyboard.press(actions2[keyIndex]);
      }
      break;

    case RELEASED:
      //Serial.println("Released " + String(key));
      Keyboard.releaseAll();                               
      break;

    // case HOLD:
      //      //Serial.println("Hold " + String(key));
      //      if (key == '+') {
      //        // blablabla...
      //      }
      //      break;
  }
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
