
// #define KEY_LEFT_CTRL   0x80
// #define KEY_LEFT_SHIFT    0x81
// #define KEY_LEFT_ALT    0x82
// #define KEY_LEFT_GUI    0x83
// #define KEY_RIGHT_CTRL    0x84
// #define KEY_RIGHT_SHIFT   0x85
// #define KEY_RIGHT_ALT   0x86
// #define KEY_RIGHT_GUI   0x87

// #define KEY_UP_ARROW    0xDA
// #define KEY_DOWN_ARROW    0xD9
// #define KEY_LEFT_ARROW    0xD8
// #define KEY_RIGHT_ARROW   0xD7
// #define KEY_BACKSPACE   0xB2
// #define KEY_TAB       0xB3
// #define KEY_RETURN      0xB0
// #define KEY_ESC       0xB1
// #define KEY_INSERT      0xD1
// #define KEY_DELETE      0xD4
// #define KEY_PAGE_UP     0xD3
// #define KEY_PAGE_DOWN   0xD6
// #define KEY_HOME      0xD2
// #define KEY_END       0xD5
// #define KEY_CAPS_LOCK   0xC1
// #define KEY_F1        0xC2
// #define KEY_F2        0xC3
// #define KEY_F3        0xC4
// #define KEY_F4        0xC5
// #define KEY_F5        0xC6
// #define KEY_F6        0xC7
// #define KEY_F7        0xC8
// #define KEY_F8        0xC9
// #define KEY_F9        0xCA
// #define KEY_F10       0xCB
// #define KEY_F11       0xCC
// #define KEY_F12       0xCD
// #define KEY_F13       0xF0
// #define KEY_F14       0xF1
// #define KEY_F15       0xF2
// #define KEY_F16       0xF3
// #define KEY_F17       0xF4
// #define KEY_F18       0xF5
// #define KEY_F19       0xF6
// #define KEY_F20       0xF7
// #define KEY_F21       0xF8
// #define KEY_F22       0xF9
// #define KEY_F23       0xFA
// #define KEY_F24       0xFB



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

byte rowPins[ROWS] = {2, 3, 4, 5}; //rows (outputs)
byte colPins[COLS] = {6, 7, 8, 9}; //columns (inputs)

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char key;

char acciones1[ROWS * COLS] = {               // primera tecla envíada al recibir una pulsación del teclado 4x4
  'q',  '2',  KEY_LEFT_CTRL,  'r',
  's',  '5',  't',  KEY_LEFT_CTRL,
  KEY_LEFT_SHIFT,  'N',  'e',  ' ',
  KEY_LEFT_SHIFT,  KEY_LEFT_CTRL,  ' ',  ' '
};

char acciones2[ROWS * COLS] = {              // si el valor es distinto de 0, segunda pulsación enviada 
  0,  0,  'd',  0,
  0,  0,  0,  'z',
  '7',  0,  0,  KEY_LEFT_SHIFT,
  0,  0,  0,  KEY_LEFT_CTRL
};



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



void loop() {
  key = keypad.getKey();               // rastrea el teclado
  delay(5);
}



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
