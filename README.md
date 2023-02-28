# Input device for drawing tablet (and MIDI device too)

![Image 1](/DOC/Img1.JPG)

This DIY device is recognized by any computer as a USB keyboard. Nor specific driver, neither other software is needed.
Inside, there is only an Arduino Pro Micro, a 4x4 keyboard matrix and 3 rotary encoders.

I use it with Krita but can be programmed with any key code combination, so it can work with any other drawing software.
The 16 keys and the 3 knobs/push buttons can send any combination of key codes.
The preprogrammed key codes can be modified easily, depending on your preferences, changing the corresponding program lines.

I have programmed the first knob for "rotate canvas" function (and reset the rotation if you press the knob)
The second knob is assigned for zooming (and reset the zoom to "fit page" pressing the knob)
The third knob is for resizing the brush. In this case, pushing the knob toggle the 'erase mode' of the brush.
The 4x4 keyboard has been programmed according to the image. In some cases, the shortcuts in my Krita setup are not the standard ones, so pay attention to this.

In this new version, a key can be configured to switch beetwen 'Keyboarde mode' and 'MIDI mode' whit a long press. In this case, the device will be recognized as a MIDI controller by the computer. All the parámeters related to MIDI mode are defined in the config file.

All the config parameters are located in the file 'Setup1.h'. It is possible to use different config files, as 'Setup2.h' for example, changing the corresponding line in the main program.For more information, schematic, etc. see the files in the DOC folder.

You can see a video with an example of use: [Example in youtube](https://youtu.be/WGJYHXOumJs)
