//
// This file is only a help to find the key codes to use
//

/*
 #define KEY_LEFT_CTRL   0x80
 #define KEY_LEFT_SHIFT    0x81
 #define KEY_LEFT_ALT    0x82
 #define KEY_LEFT_GUI    0x83
 #define KEY_RIGHT_CTRL    0x84
 #define KEY_RIGHT_SHIFT   0x85
 #define KEY_RIGHT_ALT   0x86
 #define KEY_RIGHT_GUI   0x87

 #define KEY_UP_ARROW    0xDA
 #define KEY_DOWN_ARROW    0xD9
 #define KEY_LEFT_ARROW    0xD8
 #define KEY_RIGHT_ARROW   0xD7
 #define KEY_BACKSPACE   0xB2
 #define KEY_TAB       0xB3
 #define KEY_RETURN      0xB0
 #define KEY_ESC       0xB1
 #define KEY_INSERT      0xD1
 #define KEY_DELETE      0xD4
 #define KEY_PAGE_UP     0xD3
 #define KEY_PAGE_DOWN   0xD6
 #define KEY_HOME      0xD2
 #define KEY_END       0xD5
 #define KEY_CAPS_LOCK   0xC1
 #define KEY_F1        0xC2
 #define KEY_F2        0xC3
 #define KEY_F3        0xC4
 #define KEY_F4        0xC5
 #define KEY_F5        0xC6
 #define KEY_F6        0xC7
 #define KEY_F7        0xC8
 #define KEY_F8        0xC9
 #define KEY_F9        0xCA
 #define KEY_F10       0xCB
 #define KEY_F11       0xCC
 #define KEY_F12       0xCD
 #define KEY_F13       0xF0
 #define KEY_F14       0xF1
 #define KEY_F15       0xF2
 #define KEY_F16       0xF3
 #define KEY_F17       0xF4
 #define KEY_F18       0xF5
 #define KEY_F19       0xF6
 #define KEY_F20       0xF7
 #define KEY_F21       0xF8
 #define KEY_F22       0xF9
 #define KEY_F23       0xFA
 #define KEY_F24       0xFB

USB CODE Keyboard() Key Location on US-en Keyboard
======== ========== ==============================
0 0x00              Reserved (no event indicated) *9
1 0x01              Keyboard ErrorRollOver *9 
2 0x02              Keyboard POSTFail *9 
3 0x03              Keyboard ErrorUndefined *9 
4 0x04    140 0x8C  Keyboard a and A *4 
5 0x05    141 0x8D  Keyboard b and B 
6 0x06    142 0x8E  Keyboard c and C *4 
7 0x07    143 0x8F  Keyboard d and D 
8 0x08    144 0x90  Keyboard e and E 
9 0x09    145 0x91  Keyboard f and F 
10  0x0A  146 0x92  Keyboard g and G 
11  0x0B  147 0x93  Keyboard h and H 
12  0x0C  148 0x94  Keyboard i and I 
13  0x0D  149 0x95  Keyboard j and J 
14  0x0E  150 0x96  Keyboard k and K 
15  0x0F  151 0x97  Keyboard l and L 
16  0x10  152 0x98  Keyboard m and M *4 
17  0x11  153 0x99  Keyboard n and N 
18  0x12  154 0x9A  Keyboard o and O *4 
19  0x13  155 0x9B  Keyboard p and P *4 
20  0x14  156 0x9C  Keyboard q and Q *4
21  0x15  157 0x9D  Keyboard r and R
22  0x16  158 0x9E  Keyboard s and S *4
23  0x17  159 0x9F  Keyboard t and T
24  0x18  160 0xA0  Keyboard u and U
25  0x19  161 0xA1  Keyboard v and V
26  0x1A  162 0xA2  Keyboard w and W *4
27  0x1B  163 0xA3  Keyboard x and X *4
28  0x1C  164 0xA4  Keyboard y and Y *4
29  0x1D  165 0xA5  Keyboard z and Z *4
30  0x1E  166 0xA6  Keyboard 1 and ! *4
31  0x1F  167 0xA7  Keyboard 2 and @ *4
32  0x20  168 0xA8  Keyboard 3 and # *4
33  0x21  169 0xA9  Keyboard 4 and $ *4
34  0x22  170 0xAA  Keyboard 5 and % *4
35  0x23  171 0xAB  Keyboard 6 and ^ *4
36  0x24  172 0xAC  Keyboard 7 and & *4
37  0x25  173 0xAD  Keyboard 8 and * *4
38  0x26  174 0xAE  Keyboard 9 and ( *4
39  0x27  175 0xAF  Keyboard 0 and ) *4
40  0x28  176 0xB0  Keyboard Return (ENTER) *5 
41  0x29  177 0xB1  Keyboard ESCAPE
42  0x2A  178 0xB2  Keyboard DELETE (Backspace) *13 
43  0x2B  179 0xB3  Keyboard Tab
44  0x2C  180 0xB4  Keyboard Spacebar
45  0x2D  181 0xB5  Keyboard - and (underscore) *4 
46  0x2E  182 0xB6  Keyboard = and + *4
47  0x2F  183 0xB7  Keyboard [ and { *4
48  0x30  184 0xB8  Keyboard ] and } *4
49  0x31  185 0xB9  Keyboard \ and |
50  0x32  186 0xBA  Keyboard Non-US # and ~ *2 
51  0x33  187 0xBB  Keyboard ; and : *4
52  0x34  188 0xBC  Keyboard ‘ and “ *4
53  0x35  189 0xBD  Keyboard Grave Accent and Tilde *4 
54  0x36  190 0xBE  Keyboard , and < *4
55  0x37  191 0xBF  Keyboard . and > *4
56  0x38  192 0xC0  Keyboard / and ? *4
57  0x39  193 0xC1  Keyboard Caps Lock *11
58  0x3A  194 0xC2  Keyboard F1
59  0x3B  195 0xC3  Keyboard F2
60  0x3C  196 0xC4  Keyboard F3
61  0x3D  197 0xC5  Keyboard F4
62  0x3E  198 0xC6  Keyboard F5
63  0x3F  199 0xC7  Keyboard F6
64  0x40  200 0xC8  Keyboard F7
65  0x41  201 0xC9  Keyboard F8
66  0x42  202 0xCA  Keyboard F9
67  0x43  203 0xCB  Keyboard F10
68  0x44  204 0xCC  Keyboard F11
69  0x45  205 0xCD  Keyboard F12
70  0x46  206 0xCE  Keyboard PrintScreen *1 
71  0x47  207 0xCF  Keyboard Scroll Lock *11 
72  0x48  208 0xD0  Keyboard Pause *1
73  0x49  209 0xD1  Keyboard Insert *1
74  0x4A  210 0xD2  Keyboard Home *1
75  0x4B  211 0xD3  Keyboard PageUp *1 
76  0x4C  212 0xD4  Keyboard Delete Forward *1;*14 
77  0x4D  213 0xD5  Keyboard End *1
78  0x4E  214 0xD6  Keyboard PageDown *1 
79  0x4F  215 0xD7  Keyboard RightArrow *1 
80  0x50  216 0xD8  Keyboard LeftArrow *1 
81  0x51  217 0xD9  Keyboard DownArrow *1 
82  0x52  218 0xDA  Keyboard UpArrow *1
83  0x53  219 0xDB  Keypad Num Lock and Clear *11 
84  0x54  220 0xDC  Keypad / *1
85  0x55  221 0xDD  Keypad * 
86  0x56  222 0xDE  Keypad - 
87  0x57  223 0xDF  Keypad + 
88  0x58  224 0xE0  Keypad ENTER *5
89  0x59  225 0xE1  Keypad 1 and End
90  0x5A  226 0xE2  Keypad 2 and Down Arrow
91  0x5B  227 0xE3  Keypad 3 and PageDn
92  0x5C  228 0xE4  Keypad 4 and Left Arrow
93  0x5D  229 0xE5  Keypad 5 
94  0x5E  230 0xE6  Keypad 6 and Right Arrow
95  0x5F  231 0xE7  Keypad 7 and Home
96  0x60  232 0xE8  Keypad 8 and Up Arrow
97  0x61  233 0xE9  Keypad 9 and PageUp
98  0x62  234 0xEA  Keypad 0 and Insert 
99  0x63  235 0xEB  Keypad . and Delete
100 0x64  236 0xEC  Keyboard Non-US \ and | *3;*6
101 0x65  237 0xED  Application *10
102 0x66  238 0xEE  Keyboard Power *9
103 0x67  239 0xEF  Keypad =
104 0x68  240 0xF0  Keyboard F13
105 0x69  241 0xF1  Keyboard F14
106 0x6A  242 0xF2  Keyboard F15
107 0x6B  243 0xF3  Keyboard F16
108 0x6C  244 0xF4  Keyboard F17
109 0x6D  245 0xF5  Keyboard F18
110 0x6E  246 0xF6  Keyboard F19
111 0x6F  247 0xF7  Keyboard F20
112 0x70  248 0xF8  Keyboard F21
113 0x71  249 0xF9  Keyboard F22
114 0x72  250 0xFA  Keyboard F23
115 0x73  251 0xFB  Keyboard F24
116 0x74  252 0xFC  Keyboard Execute
117 0x75  253 0xFD  Keyboard Help
118 0x76  254 0xFE  Keyboard Menu
119 0x77  255 0xFF  Keyboard Select 

 Keyboard.press(KEY_RIGHT_ALT);
 Keyboard.write (Keypad_0);
 Keyboard.write (Keypad_1);
 Keyboard.write (Keypad_9);
 Keyboard.write (Keypad_4);
 Keyboard.release(KEY_RIGHT_ALT);
*/
