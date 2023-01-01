# Installation Instructions #
You can either use the pre-compiled .uf2 files (inside the build directory) or compile the source code yourself.

### Pre-compiled .uf2 files ###
1. Download the correct .uf2 file for your hardware from the Releases tab. Pre-compiled .uf2 files support the following hardware combinations:
    * Pico + No Display
    * Picow + No Display
    * Pico + Pico Display 2
    * Picow + Pico Display 2 
2. Plug your Pico into your computer in bootloader mode.
3. Drag and drop your chosen .uf2 file onto the Pico's drive.
4. That's it! Your Pico will show as a USB device on your computer.

### Compiling the source code yourself ###
1. Download the source code onto your computer.
2. Download the Arduino IDE from https://www.arduino.cc/en/software
3. Open the Arduino IDE and go to File->Preferences and add the following URL to "Additional Boards Manager URLs": 
`https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json` 
Hit "OK".
4. Open the source code in the Arduino IDE by double clicking the picoduck.ino file.

**Arduino IDE Setup - This is important to get right!**

5. Go to Tools->Board->Boards Manager and search for "Raspberry Pi Pico/RP2040". Install the "RP2040 Boards by earlephilhower" package
6. Go to Tools->Board->Raspberry Pi Pico/RP2040 and select your board (Raspberry Pi Pico or Raspberry Pi Pico W)
7. Go to Tools->USB Stack and select "Adafruit TinyUSB"
8. Go to Tools->Manage Libraries and search for "Adafruit SPIFlash". Install the "Adafruit SPIFlash" library.

**Now the Arduino IDE should be correctly setup!**

9. Now you can plug your Pico into your computer in bootloader mode. You can use the Arduino IDE upload feature, or you can press Sketch -> Export Compiled Binary (CTRL+ALT+S) and manually drag and drop the compiled .uf2 file (inside build directory!) onto the Pico's drive.
10. That's it! Your Pico will show as a USB device on your computer.

## PICO DISPLAY 2 USAGE INSTRUCTIONS ##
```
                +--------PICO DISPLAY 2---------+
(SCRLL UP)    A |PICOUSB      | WIFI: 0.0.0.0   | X  (CYCLE)
(RMV AUTORUN)   |-------------------------------|
                |COMMANDS     | OUTPUT          |
                |             |                 |
                |notepad      | hello ....      |
                |script.dd    | .....           |
                |             |       ...  ..   |
(SCRLL DWN)   B |             |                 | Y  (EXECUTE)
(ADD AUTORUN)   +--------------------------------+
```

**Button A:**
When a **COMMAND** is selected, the **AUTORUN** file will be cleared, so that no command is autoran on the next boot.

When **OUTPUT** is selected, the output will scroll up.

**Button B:**
When a **COMMAND** is selected, that command will be **autoran** on the next boot of the device.

When **OUTPUT** is selected, the output will scroll down.

**Button X:** 
This will cycle through **COMMANDS** and **OUTPUT**. (Down only)
e.g. notepad (**X**) script.dd (**X**) output (**X**) notepad

**Button Y:** 
When a **COMMAND** is selected, that command will be **autoran** on the next boot of the device.

(Nothing happens when **OUTPUT** is selected.)

