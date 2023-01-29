//
// Setup1 (Define in the main tab the "SetupX.h" to use)
//

// PINs 2,3 are used by Wire library (SDA, SCL in Display)
boolean bUseDisplay = false;

boolean bUseMIDI =false;
#define START_IN_MIDI_MODE false;

#define pin_LED_Status A2

#define pin_LED_TestMode A2

boolean bUseLEDTestMode = false;

///////////////////////////////////////////////////////////////////// 4x4 Keyboard /////////////////////////////////////////

// pins to use in 4x4 keyboard
byte rowPins[ROWS] = {7, 8, 9, 10}; //rows
byte colPins[COLS] = {3, 4, 5, 6};  //columns

int index_LocalShiftKey = 3;                // index from 0 to 15 of the key that acts a 'shift' key. If -1 then no localShiftKey 

// Actions for 4x4 keyboard
const char actions[ROWS * COLS][2] = {  // code sent when a key is pressed. if second value is other than 0 send it
  {'q', 0},            {'t', 0},             {KEY_LEFT_CTRL, 'd'},{'|', 0},
  {KEY_TAB, 0},        {'s', 0},             {'r', 0},            {KEY_BACKSPACE, 0},
  {KEY_LEFT_SHIFT, 0}, {'N', 0},             {'z', 0},            {KEY_LEFT_CTRL, ' '},
  {KEY_LEFT_CTRL, 0},  {KEY_LEFT_CTRL, 'z'}, {'m', 0},   {' ',0}
};

const char ACT_LABEL_1[] PROGMEM = "FreeHand\nSelectTmp.";
const char ACT_LABEL_2[] PROGMEM = "Move temp.";
const char ACT_LABEL_3[] PROGMEM = "Unselect\nAll";
const char ACT_LABEL_4[] PROGMEM = "LocalShift";
const char ACT_LABEL_5[] PROGMEM = "Only\ncanvas";
const char ACT_LABEL_6[] PROGMEM = "Isolate\nlayer";
const char ACT_LABEL_7[] PROGMEM = "Select\nlayer";
const char ACT_LABEL_8[] PROGMEM = "BackSpace\nFillBbgCol";
const char ACT_LABEL_9[] PROGMEM = "Shift\nResiz.Brsh";
const char ACT_LABEL_10[] PROGMEM = "ShadeColor\nselector";
const char ACT_LABEL_11[] PROGMEM = "Pie menu";
const char ACT_LABEL_12[] PROGMEM = "Zoom\n(Ctrl+SPC)";
const char ACT_LABEL_13[] PROGMEM = "Color pick\n(Ctrl)";
const char ACT_LABEL_14[] PROGMEM = "Undo\n(Ctrl Z)";
const char ACT_LABEL_15[] PROGMEM = "Mirror\n(M)";
const char ACT_LABEL_16[] PROGMEM = "SPC";

const char* const actions_labels[ROWS * COLS] = {
    ACT_LABEL_1,
    ACT_LABEL_2,
    ACT_LABEL_3,
    ACT_LABEL_4,
    ACT_LABEL_5,
    ACT_LABEL_6,
    ACT_LABEL_7,
    ACT_LABEL_8,
    ACT_LABEL_9,
    ACT_LABEL_10,
    ACT_LABEL_11,
    ACT_LABEL_12,
    ACT_LABEL_13,
    ACT_LABEL_14,
    ACT_LABEL_15,
    ACT_LABEL_16
};


// Next actions are active when localShiftKey has been pressed, insted of the 'actions' array. 
const char actions_LocalShift[ROWS * COLS][2] = {  // code sent when a key is pressed. if second value is other than 0 send it
  {KEY_LEFT_CTRL, 't'}, {KEY_LEFT_CTRL, 'u'}, {KEY_F3, 0},          {'|',0},
  {KEY_LEFT_CTRL, 'a'}, {KEY_LEFT_CTRL, 'g'}, {KEY_LEFT_ALT, 'g'},  {0xB0, 'b'},
  {'x', 0},             {KEY_LEFT_CTRL, 'b'}, {KEY_LEFT_CTRL, 'k'}, {KEY_LEFT_CTRL, 'l'},
  {KEY_LEFT_CTRL, 'x'}, {KEY_LEFT_CTRL, 'c'}, {KEY_LEFT_CTRL, 'v'}, {KEY_LEFT_SHIFT, 'v'}
};

const char ACT_SHIFT_LABEL_1[] PROGMEM = "Transform\nTool\n(Ctrl T)";
const char ACT_SHIFT_LABEL_2[] PROGMEM = "HVS\nadjust\n(Ctrl U)";
const char ACT_SHIFT_LABEL_3[] PROGMEM = "Properties\nF3";
const char ACT_SHIFT_LABEL_4[] PROGMEM = "LocalShift";
const char ACT_SHIFT_LABEL_5[] PROGMEM = "Select all\n(Ctrl A)";
const char ACT_SHIFT_LABEL_6[] PROGMEM = "Group\n(Ctrl G)";
const char ACT_SHIFT_LABEL_7[] PROGMEM = "Clip Group\n(Alt G)";
const char ACT_SHIFT_LABEL_8[] PROGMEM = "Enter + B\n(Enter and\nsel.Brush)";
const char ACT_SHIFT_LABEL_9[] PROGMEM = "Exch Fg/Bg\ncolor\n(X)";
const char ACT_SHIFT_LABEL_10[] PROGMEM = "ColBalance\n(Ctrl B)";
const char ACT_SHIFT_LABEL_11[] PROGMEM = "Invert\nselection\n(Ctrl K)";
const char ACT_SHIFT_LABEL_12[] PROGMEM = "Levels\n(Ctrl L)";
const char ACT_SHIFT_LABEL_13[] PROGMEM = "Cut\n(Ctrl X)";
const char ACT_SHIFT_LABEL_14[] PROGMEM = "Copy\n(Ctrl C)";
const char ACT_SHIFT_LABEL_15[] PROGMEM = "Paste\n(Ctrl v))";
const char ACT_SHIFT_LABEL_16[] PROGMEM = "Paste in\nsame layer";

const char* const actions_LocalShift_labels[ROWS * COLS] = {
    ACT_SHIFT_LABEL_1,
    ACT_SHIFT_LABEL_2,
    ACT_SHIFT_LABEL_3,
    ACT_SHIFT_LABEL_4,
    ACT_SHIFT_LABEL_5,
    ACT_SHIFT_LABEL_6,
    ACT_SHIFT_LABEL_7,
    ACT_SHIFT_LABEL_8,
    ACT_SHIFT_LABEL_9,
    ACT_SHIFT_LABEL_10,
    ACT_SHIFT_LABEL_11,
    ACT_SHIFT_LABEL_12,
    ACT_SHIFT_LABEL_13,
    ACT_SHIFT_LABEL_14,
    ACT_SHIFT_LABEL_15,
    ACT_SHIFT_LABEL_16
};



////////////////////////////////////////////////////////////// ROTARY ENCODERS /////////////////////////////////////////////////////

// Change these two numbers to the PINs connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc0(2, 16);
Encoder myEnc1(0, 14);
Encoder myEnc2(1, 15);
//   avoid using pins with LEDs attached

// Analog PIN for use with the three encoder buttons
const int ENCODER_BUTTONS_ANALOG_PIN = A3;

// Actions for rotary encoders
const char actions1Encoders[N_ENCODERS][2] = {       // Codes sent by the encoders (index [n][INX_ENCODER_UP]=UP or index [n][INX_ENCODER_DOWN]=DOWN)
  {'4',  '6'},                                 // Rotation
  {222,  223},                                 // 223=+ 222=-  Zoom
  {'7',  '9'}                                  // Brush size (not standard krita shortcut. Must be assigned in krita too)
};

const char actions2Encoders[N_ENCODERS][2] = {       // second code to send, if needed (the value is other than 0)
  {0,  0},
  {0,  0}, 
  {0,  0}
};


// Actions for encoder_buttons
const char actionsEncoder_Buttons[N_ENCODERS][2] = { // First key sent for each encoder_button. if other than 0, second key sent for each encoder_button
  {'5', 0},   // Reset rotation
  {'2', 0},   // Reset zoom (fit to page)
  {'e', 0}    // Toggle eraser mode
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

boolean testModeEnabled = true;
boolean sendInfoToComputerInTestMode = false;
