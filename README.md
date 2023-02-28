# Input device for drawing tablet (and now, MIDI device too)

![Image 1](/DOC/Img1.JPG)

This DIY device is recognized by any computer as a USB keyboard. Nor specific driver, neither other software is needed.
Inside, there is only an Arduino Pro Micro, a 4x4 keyboard matrix and 3 rotary encoders.

I use it with Krita but can be programmed with any key code combination, so it can work with any other drawing software.
The 16 keys and the 3 knobs/push buttons can send any combination of key codes.
The preprogrammed key codes can be modified easily, depending on your preferences, changing the corresponding program lines.

I have programmed the first knob for "rotate canvas" function (and reset the rotation if you press the knob)


The second knob is assigned for zooming (and reset the zoom to "fit page" pressing the knob)

The third knob is for resizing the brush. In this case, pushing the knob toggle the 'erase mode' of the brush.

Bear in mind that, in some cases, the shortcuts in my Krita setup are not the standard ones, so pay attention to this.

In this new version, a display shows the info about what key or action is performed. 

In addition, a key (any of the 4x4) can be defined as 'local shift', which allow accessing to a different set of configurations for the rest of 15 keys.

Pressing once 'local shift' the system enters in temporary 'local shift' mode (the LED blinks), and after the next stroke in any key, the system will return to normal mode (and the LED turns off).

Pressing twice 'local shift' the system enters in 'local shift' mode (the LED turns on) , and remains in this mode until deactivated by a new press on 'local shift' and the system will return to normal mode (and the LED turns off).

A long press on 'local shift' will switch beetwen 'normal mode' and 'Test mode' (the LED blinks less frequently). While in 'test mode' no keystrokes are sent to the computer but displayed on the screen. Handy for checking how all the keys are configured. To exit 'test mode', long-press 'local shift' again.

A long press (more than 2 seconds) on 'Knob #1' will switch beetwen 'Keyboard mode' and 'MIDI mode'. In this case, the device will be recognized as a MIDI controller by the computer. All the parameters related to MIDI mode are defined in the setup file and are independent of the keyboard mode.

All the config parameters are located in the file 'Setup1.h'. It is possible to use different config files, such as 'Setup2.h'... etc., changing the corresponding line in the main program (just in case you have several devices with different configurations).

For more information, schematic, etc. see the files in the DOC folder.

You can see a video with an example of use: [Example in youtube](https://youtu.be/WGJYHXOumJs)
