
///////////////////////////////////////////////////////////////////////////////    TECLADO 4X4   ///////////////////////////////////////////////////////

#include <Keyboard.h>               // librería para el teclado USB hacia ordenador       

#include <Keypad.h>                 // librería para el control de teclado físico 4x4
const byte ROWS = 4; // rows
const byte COLS = 4; // columns
char keys[ROWS][COLS] = {           // los caracteres a partir del 9 son la continuación de la tabla ASCII para facilitar la indexación (de 0 a 15)
  {'0', '1', '2', '3'},
  {'4', '5', '6', '7'},
  {'8', '9', ':', ';'},
  {'<', '=', '>', '?'}
};

byte rowPins[ROWS] = {7, 8, 9, 10}; //rows (outputs)
byte colPins[COLS] = {3, 4, 5, 6}; //columns (inputs)

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char key;

//char acciones1[ROWS * COLS] = {               // primera tecla envíada al recibir una pulsación del teclado 4x4
//  'q',  't',  KEY_LEFT_CTRL,  ' ',
//  's',  'r',  KEY_BACKSPACE,  KEY_LEFT_SHIFT,
//  'x',  'N',  'v',  KEY_TAB,
//  KEY_DELETE,  KEY_LEFT_CTRL,  KEY_LEFT_ALT,  KEY_LEFT_CTRL
//};
//
//char acciones2[ROWS * COLS] = {              // si el valor es distinto de 0, segunda pulsación enviada 
//  0,  0,  'd',  0,
//  0,  0,  0, KEY_BACKSPACE,
//  0,  0,  0,  KEY_LEFT_SHIFT,
//  0,  'z', 'z',  0
//};

char acciones1[ROWS * COLS] = {               // primera tecla envíada al recibir una pulsación del teclado 4x4
  KEY_DELETE,  'q', 't',  KEY_LEFT_CTRL,
  KEY_TAB, 's',  'r',  KEY_BACKSPACE,
  KEY_LEFT_SHIFT,  'N',  'x',  'v',
  KEY_LEFT_CTRL,  KEY_LEFT_CTRL,  KEY_LEFT_ALT,  ' '
};

char acciones2[ROWS * COLS] = {              // si el valor es distinto de 0, segunda pulsación enviada 
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

char acciones1Encoders[N_ENCODERS][2] = {       // primera tecla envíada por cada encoder (indice[n][INX_ENCODER_UP]=UP o indice [n][INX_ENCODER_DOWN]=DOWN)
  {'4',  '6'}, // Rotation
  {222,  223}, // 223=+ 222=-  Zoom
  {'7',  '9'}  // Brush size
};

char acciones2Encoders[N_ENCODERS][2] = {       // si el valor es distinto de 0, segunda tecla envíada por cada encoder (indice[n][INX_ENCODER_UP]=UP o indice [n][INX_ENCODER_DOWN]=DOWN)
  {0,  0},
  {0,  0}, 
  {0,  0}
};


//////////////////////////////////////////////////////////////////////   TECLADO ANALOGICO (3 TECLAS EN UNA ENTRADA ANALÓGICA)  ////////////////////////

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

char acciones1Encoder_Buttons[N_ENCODERS] = {       // primera tecla envíada por cada encoder_button
  '5',   // Rotación 0
  '2',   // Zoom pantalla
  'e'    // Borrador
};

char acciones2Encoder_Buttons[N_ENCODERS] = {       // si el valor es distinto de 0, segunda tecla envíada por cada encoder_button
  0,
  0,
  0
};



/////////////////////////////////////////////////////// setup //////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  Serial.println("Iniciado.");

  //apagar los leds del puerto serie definiéndolos como entradas
  pinMode(LED_BUILTIN_TX, INPUT);
  pinMode(LED_BUILTIN_RX, INPUT);

  keypad.setDebounceTime(50);
  keypad.addEventListener(keypadEvent); // Add an event listener for this keypad

  Keyboard.begin();
}




/////////////////////////////////////////////////////// loop //////////////////////////////////////////////////////////////

void loop() {
  // rastrea el teclado; si hay pulsaciones las tratará la función 'keypadEvent' 
  key = keypad.getKey();                   
  //
  
  // lee los encoders 
  long newPositionEncoder;              

  for (int nEncoder=0; nEncoder < N_ENCODERS ; nEncoder++) {
    //boolean hayCambioEnEncoder = false;
    int indiceUpDown = -1;
    
    newPositionEncoder = leerEncoderN(nEncoder);
    if (newPositionEncoder > 1) indiceUpDown=INX_ENCODER_UP;
    if (newPositionEncoder < -1) indiceUpDown=INX_ENCODER_DOWN;
    if (indiceUpDown != -1) {
        Keyboard.press(acciones1Encoders[nEncoder][indiceUpDown]);
        if (acciones2Encoders[nEncoder][indiceUpDown] != 0) {                    // si la segunda acción es distinta de 0, la envía
          Keyboard.press(acciones2Encoders[nEncoder][indiceUpDown] );
        }
        resetearEncoderN(nEncoder);
        Keyboard.releaseAll();
    }
  }
  //
  

  // leer los botones de los encoders  ////////////////////////////////////////////////////////////////////////////////////////
  encoder_buttons.update();                                     
  
  if(encoder_buttons.onPressAndAfter(ENCODER0_BUTTON, 0)) {     // 0= no hacer nada tras la primera pulsación
    Serial.println("btn0");
    Keyboard.press(acciones1Encoder_Buttons[0]);
    if (acciones2Encoder_Buttons[0] != 0) {                    // si la segunda acción es distinta de 0, la envía
      Keyboard.press(acciones2Encoder_Buttons[0]);
    }
    //Keyboard.releaseAll();
  }
  
  if(encoder_buttons.onPressAndAfter(ENCODER1_BUTTON, 0)) {     // 0= no hacer nada tras la primera pulsación
    Serial.println("btn1");
    Keyboard.press(acciones1Encoder_Buttons[1]);
    if (acciones2Encoder_Buttons[0] != 1) {                    // si la segunda acción es distinta de 0, la envía
      Keyboard.press(acciones2Encoder_Buttons[1]);
    }
    //Keyboard.releaseAll();    
  }
  
  if(encoder_buttons.onPressAndAfter(ENCODER2_BUTTON, 0)) {     // 0= no hacer nada tras la primera pulsación
    Serial.println("btn2");
    Keyboard.press(acciones1Encoder_Buttons[2]);
    if (acciones2Encoder_Buttons[2] != 0) {                    // si la segunda acción es distinta de 0, la envía
      Keyboard.press(acciones2Encoder_Buttons[2]);
    }
    //Keyboard.releaseAll();   
  }
  
  // comprobar si se ha soltado algún pulsador
  if(encoder_buttons.onRelease(ENCODER0_BUTTON) || encoder_buttons.onRelease(ENCODER1_BUTTON) || encoder_buttons.onRelease(ENCODER2_BUTTON)) {
    Keyboard.releaseAll();
  }
}
    



/////////////////////////////////////////////////////// funciones //////////////////////////////////////////////////////////////

void keypadEvent(KeypadEvent key) {
  int indiceTecla = key - char('0');   // obtiene el índice de la pulsación en la matriz (de 0 a 15)

//  //para debug
//  String cad = "Evento teclado " + String(key);
//  Serial.println(cad);
//  Serial.println(indiceTecla);
//  //

  switch (keypad.getState()) {
    case PRESSED:
      //Serial.println(String("Accion1 " + String(key)));
      Keyboard.press(acciones1[indiceTecla]);               // envía la primera acción correspondientea a la tecla pulsada
      if (acciones2[indiceTecla] != 0) {                    // si la segunda acción es distinta de 0, la envía
        //Serial.println(String("Accion2 "  + String(key)));
        Keyboard.press(acciones2[indiceTecla]);
      }
      break;

    case RELEASED:
      //Serial.println("Released " + String(key));
      Keyboard.releaseAll();                                // envía la liberación de cualquier pulsación que hubiera
      break;

    // case HOLD:
      //      //Serial.println("Hold " + String(key));
      //      if (key == '+') {
      //        // blablabla...
      //      }
      //      break;
  }
}



long leerEncoderN(int nEncoder) {
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



void resetearEncoderN(int nEncoder) {
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
