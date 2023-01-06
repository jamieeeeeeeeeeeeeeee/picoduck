# Information #
This project is a USB Rubber Ducky clone for the Raspberry Pi Picow. **Please note that it currently relies on the Picow and having a Pimoroni Pico Display 2 - I am working on adding configuration options so you can use your own display (and no display!), and the base Pico.**

# Installation Instructions #
You can either use the pre-compiled .uf2 files (from the Releases tab) or compile the source code yourself.

## Pre-compiled .uf2 files ##
1. Download the correct .uf2 file for your hardware from the Releases tab. Pre-compiled .uf2 files support the following hardware combinations:
    * ~~No Display + Pico~~
    * ~~No Display + Picow~~
    * ~~Pico Display 2 + Pico~~
    * Pico Display 2 + Picow
2. Plug your Pico into your computer in bootloader mode.
3. Drag and drop your chosen .uf2 file onto the Pico's drive.
4. That's it! Your Pico will show as a USB device on your computer.

## Compiling the source code yourself ##
1. Download the source code onto your computer.
2. Download the Arduino IDE from https://www.arduino.cc/en/software

**Arduino IDE Setup - This is important to get right!**

3. Open the Arduino IDE and go to File->Preferences and add the following URL to "Additional Boards Manager URLs": 
`https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json` 
Hit "OK".
4. Open the source code in the Arduino IDE by double clicking the picoduck.ino file.
5. Go to Tools->Board->Boards Manager and search for "Raspberry Pi Pico/RP2040". Install the "RP2040 Boards by earlephilhower" package
6. Go to Tools->Board->Raspberry Pi Pico/RP2040 and select your board (Raspberry Pi Pico or Raspberry Pi Pico W)
7. Go to Tools->USB Stack and select "Adafruit TinyUSB"
8. Go to Tools->Manage Libraries and search for "Adafruit SPIFlash". Install the "Adafruit SPIFlash" library.

**Now the Arduino IDE should be correctly setup!**

9. Now you can plug your Pico into your computer in bootloader mode. You can use the Arduino IDE upload feature, or you can press Sketch -> Export Compiled Binary (CTRL+ALT+S) and manually drag and drop the compiled .uf2 file (inside build directory!) onto the Pico's drive.
10. That's it! Your Pico will show as a USB device on your computer.

# Usage Instructions #
### General USB ###
When you plug in your Pico (after flashing the .uf2 file), it will show as a USB HID (Human Interface Device) and a mass storage device.

### No Display ###
### Pico Display 2 ###
```
    +--------PICO DISPLAY 2---------+
  A |PICOUSB      | WIFI: 0.0.0.0   | X  
    |-------------------------------|
    |COMMANDS     | OUTPUT          |
    |             |                 |
    |notepad      | hello ....      |
    |script.dd    | .....           |
    |             |       ...  ..   |
  B |             |                 | Y  
    +--------------------------------+
```

| Button | Action if a Command is selected | Action if Output is selected |
|--------|---------------------------------|------------------------------|
| A      | Turn autorun off                | Scroll up                    |
| B      | Turn autorun on (this command)  | Scroll down                  |
| X      | Cycle (commands + output)       | Cycle   (commands + output)  |
| Y      | Execute (this command)          | N/A                          |

# Documentation #
The project is split into 4 main sections:
1. The main program (picoduck.ino)
2. The DuckyScript interpreter (ducky.hpp)
3. Custom commands, (C++ not Ducky, so you can do anything you want and access the Pico's hardware) (commands.hpp)
4. Configuration (config.hpp)

### The Main File (picoduck.ino) ###
This main file which handles USB communication, autorunning scripts, the display, and buttons.

### The DuckyScript Interpreter (ducky.hpp) ###
This file interprets DuckyScripts and executes them. It uses this command list for reference > https://docs.hak5.org/hak5-usb-rubber-ducky/duckyscript-tm-quick-reference 

Supported commands are:
* REM 
* STRING / STRINGLN
* UPARROW / DOWNARROW / LEFTARROW / RIGHTARROW
* INSERT / DELETE / BACKSPACE
* TAB + SPACE + ENTER +ESCAPE
* PAUSE / BREAK
* PRINTSCREEN
* Function Keys (F1-F12)
* SHIFT + ALT
* CONTROL / CTRL
* WINDOWS / GUI
* CAPSLOCK + NUMSLOCK + SCROLLOCK
* DELAY

Unsupported commands:
* MENU / APP 
* Mouse support is implemented, but commands are not (yet!)
* DEL
* DEFAULT_DELAY / DEFAULTDELAY
* The Button / LED / ATTACKMODE / Logical Operations etc..

### Custom Commands (commands.hpp) ###
Here you can write your own commands in C/C++ (these of course allow access to the Picos hardware, so can be more advanced than just DuckyScript). An example of this is connecting to WiFi.

### Configuration ###
(No configuration added yet, other than turning the display off to do this change `#define USB_DISPLAY PICO_DISPLAY_2` to `#define USB_DISPLAY NO_DISPLAY`. (Inside the config.hpp file!) More to come!

# Acknowledgements # 
**Thanks to @xslendik for providing the initial picoduck code!**

