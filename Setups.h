
// Uncommnet only ONE of the next lines (for the setup active)

#define setup1
//#define setup2
//#define setup3
//#define setup4
//#define setup5



#ifdef setup1 /////////////////////////////////////////////////////////////////////////////////////////////////////

// Actions for 4x4 keyboard
char actions[ROWS * COLS][2] = {  // code sent when a key is pressed. if second value is other than 0 send it
  {'q', 0},            {'t', 0},             {KEY_LEFT_CTRL, 'd'},{'|', 0},
  {KEY_TAB, 0},        {'s', 0},             {'r', 0},            {KEY_BACKSPACE, 0},
  {KEY_LEFT_SHIFT, 0}, {'N', 0},             {'z', 0},            {KEY_LEFT_CTRL, ' '},
  {KEY_LEFT_CTRL, 0},  {KEY_LEFT_CTRL, 'z'}, {'m', 0},   {' ',0}
};

String actions_labels[ROWS * COLS] = {  // label to be displayed
  {"FreeHand\nSelect tmp."}, {"Move temp."},           {"Unselect\nAll"}, {"LocalShift"},
  {"Only canvas"},           {"Isolate\nlayer"},       {"Select\nlayer"}, {"BackSpace\nFill bgCol."},
  {"Shift\nResiz.Brsh"},     {"ShadeColor\nselector"}, {"Pie menu"},      {"Zoom\n(Ctrl+SPC)"},
  {"Color pick\n(Ctrl)"},    {"Undo\n(Ctrl Z)"},       {"Mirror\n(M)"},   {"SPC"}
};



// Next actions are active when localShiftKey has been pressed, insted of the 'actions' array. 
char actions_LocalShift[ROWS * COLS][2] = {  // code sent when a key is pressed. if second value is other than 0 send it
  {KEY_LEFT_CTRL, 't'}, {KEY_LEFT_CTRL, 'u'}, {KEY_F3, 0},          {'|',0},
  {KEY_LEFT_CTRL, 'a'}, {KEY_LEFT_CTRL, 'g'}, {KEY_LEFT_ALT, 'g'},  {0xB0, 'b'},
  {'x', 0},             {KEY_LEFT_CTRL, 'b'}, {KEY_LEFT_CTRL, 'k'}, {KEY_LEFT_CTRL, 'l'},
  {KEY_LEFT_CTRL, 'x'}, {KEY_LEFT_CTRL, 'c'}, {KEY_LEFT_CTRL, 'v'}, {KEY_LEFT_SHIFT, 'v'}
};

String actions_LocalShift_labels[ROWS * COLS] = {  // label to be displayed
  {"TransfTool\n(Ctrl T)"},  {"HVS adj.\n(Ctrl U)"},   {"Properties\nF3"},       {"LocalShift"},
  {"Select all\n(Ctrl A)"},  {"Group\n(Ctrl G)"},      {"ClipGroup\n(Alt G)"},   {"Enter + B\n(Enter and\nsel.Brush)"},
  {"Exch.Fg/Bg\ncolor\n(X)"},{"ColBalance\n(Ctrl B)"},  {"InvertSelec\n(Ctrl K)"},{"Levels\n(Ctrl L)"},
  {"Cut\n(Ctrl X)"},         {"Copy\n(Ctrl C)"},       {"Paste\n(Ctrl v))"},     {"Paste in\nsame layer\(Shift V)"}
};



int index_LocalShiftKey = 3;                // index from 0 to 15 of the key that acts a 'shift' key. If -1 then no localShiftKey 



// Actions for rotary encoders
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



// Actions for encoder_buttons
char actionsEncoder_Buttons[N_ENCODERS][2] = { // First key sent for each encoder_button. if other than 0, second key sent for each encoder_button
  {'5', 0},   // Reset rotation
  {'2', 0},   // Reset zoom (fit to page)
  {'e', 0}    // Toggle eraser mode
};

#endif  // setup1



#ifdef setup2 /////////////////////////////////////////////////////////////////////////////////////////////////////

// Actions for 4x4 keyboard
char actions[ROWS * COLS][2] = {  // code sent when a key is pressed. if second value is other than 0 send it
  {'q', 0},            {'t', 0},             {KEY_LEFT_CTRL, 'd'},{'|', 0},
  {KEY_TAB, 0},        {'s', 0},             {'r', 0},            {KEY_BACKSPACE, 0},
  {KEY_LEFT_SHIFT, 0}, {'N', 0},             {'z', 0},            {KEY_LEFT_CTRL, ' '},
  {KEY_LEFT_CTRL, 0},  {KEY_LEFT_CTRL, 'z'}, {'m', 0},   {' ',0}
};

// Next actions are active when localShiftKey has been pressed, insted of the 'actions' array. 
char actions_LocalShift[ROWS * COLS][2] = {  // code sent when a key is pressed. if second value is other than 0 send it
  {KEY_LEFT_CTRL, 't'}, {KEY_LEFT_CTRL, 'u'}, {KEY_F3, 0},          {'|',0},
  {KEY_LEFT_CTRL, 'a'}, {KEY_LEFT_CTRL, 'g'}, {KEY_LEFT_ALT, 'g'},  {0xB0, 'b'},
  {'x', 0},             {KEY_LEFT_CTRL, 'b'}, {KEY_LEFT_CTRL, 'k'}, {KEY_LEFT_CTRL, 'l'},
  {KEY_LEFT_CTRL, 'x'}, {KEY_LEFT_CTRL, 'c'}, {KEY_LEFT_CTRL, 'v'}, {KEY_LEFT_SHIFT, 'v'}
};

int index_LocalShiftKey = 3;                // index from 0 to 15 of the key that acts a 'shift' key. If -1 then no localShiftKey 

// Actions for rotary encoders
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

// Actions for encoder_buttons
char actionsEncoder_Buttons[N_ENCODERS][2] = { // First key sent for each encoder_button. if other than 0, second key sent for each encoder_button
  {'5', 0},   // Reset rotation
  {'2', 0},   // Reset zoom (fit to page)
  {'e', 0}    // Toggle eraser mode
};

#endif  // setup2



// etc. (for definition of other setups)
