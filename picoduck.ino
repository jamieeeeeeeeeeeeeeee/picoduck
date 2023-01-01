#include "picoduck.hpp"
#include "ducky.hpp"
#include "commands.hpp"

// setup
void setup() {
  // check for autorun
  autorun = false;
  #if USB_DISPLAY == PICO_DISPLAY_2
  if (!(button_a.raw() || button_b.raw() || button_x.raw() || button_y.raw()))
  /*
  #elif USB_DISPLAY == CUSTOM_DISPLAY
  // define your method of checking for autorun here
  */
  #endif
    autorun = true;
  #if USB_DISPLAY == PICO_DISPLAY_2
  // turn on backlight so we know autorun check is complete (and we no longer have to hold button)
  display.set_backlight(255);
  #endif

  // define globals
  WiFi.mode(WIFI_STA);
  wifi_status = WIFI_NOT_CONNECTED;
  ip_address = "0.0.0.0";

  #if USB_DISPLAY == PICO_DISPLAY_2
  selected = 0;
  scroll = 60; // this value will have to be modified based on display size
  lastbutton = 0;
  BLACK = graphics.create_pen(0, 0, 0);
  WHITE = graphics.create_pen(255, 255, 255);
  COLOUR = graphics.create_pen(176, 196, 222);
  /*
  #elif USB_DISPLAY == CUSTOM_DISPLAY
  selected = 0;
  scroll = 60; // reasonable value for start of output
  lastbutton = 0;
  // also define your colours for use in draw_functions
  */
  #endif

  // init msc
  flash.begin();
  usb_msc.setID(DISK_LABEL, "USB Storage", "2.0");
  usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);
  usb_msc.setCapacity(flash.size() / 512, 512);
  usb_msc.setUnitReady(true);
  usb_msc.begin();
  
  // init fatfs
  fs_formatted = fatfs.begin(&flash);

  // format_fat12(); // uncomment this line to confirm DISK_LABEL name change

  if (!fs_formatted) {
    // if not formatted, format
    format_fat12();

    fs_formatted = fatfs.begin(&flash);
    if (!fs_formatted) {
    }
  }

  // init keyboard and mouse
  Keyboard.begin();
  Mouse.begin();

  draw_blank_screen();
  draw_navbar();
  draw_output();
  
  // add ducky scripts to commands
  File32 ducks = fatfs.open("ducks");
  if (ducks) {
    while (true) {
      File32 duck = ducks.openNextFile();
      if (!duck) break;
      if (duck) {
        char filename[FF_MAX_LFN + 1]; // since ducks/filename this shouldn't be an issue
        duck.getName(filename, sizeof(filename));
        duck.close();
        const char *filename_c = filename;
        std::string filename_s(filename_c);
        commands[filename_s] = run_duck;
      }
    }
    ducks.close();
  }
  draw_commands();

  // run autorun command (and wait (AUTORUN_DELAY) seconds for host to register MSC)
  if (autorun) 
  {
    File32 autorun_file = fatfs.open("autorun.txt", FILE_READ);
    if (autorun_file) {
      int length = autorun_file.available();
      char contents[length+1] = { '\0' };
      autorun_file.read(contents, length);
      autorun_file.close();

      int command_start = 0;
      int command_end = 0;
      for (int i = 0; i < length; i++) {
        if (contents[i] == ';') {
          command_start = command_end;
          command_end = i;
        }
      }
      String autorun_command = String(contents).substring(command_start+1, command_end);
      
      if (commands.find(autorun_command.c_str()) != commands.end()) {
        print("Autorun command found");
        println(autorun_command.c_str());
        delay(AUTORUN_DELAY);
        // check if autorun_command ends in .dd
        if (autorun_command.endsWith(".dd")) {
          selected_duck = autorun_command.c_str();
          run_duck();
        }
        else {
          commands[autorun_command.c_str()]();
        }
      }
      else
        println("NO AUTORUN COMMAND");
    }
    else 
      println("NO AUTORUN FILE");
  }
}

// loop
void loop () { 
  #if USB_DISPLAY == PICO_DISPLAY_2
  if (pimoroni::millis() - lastbutton > DEBOUNCE)
  {
    if (button_a.raw()) {
      if (selected == -1)
      {
        scroll -= 10;
        draw_output(true);
        lastbutton = pimoroni::millis() - 175;
      }
      else
      {
        print("Removed autorun");
        File32 autorun = fatfs.open("autorun.txt", FILE_WRITE);
        autorun.write(";_X_;", 5);
        autorun.close();
        lastbutton = pimoroni::millis();
      }
    }

    if (button_b.raw()) {
      if (selected == -1)
      {
        scroll += 10;
        if (scroll > 210)
          scroll = 210;
        draw_output(true);
        lastbutton = pimoroni::millis() - 175;
      }
      else {
        print("Changed autorun to ");
        auto it = commands.begin();
        std::advance(it, selected);
        println(it->first.c_str());

        File32 autorun = fatfs.open("autorun.txt", FILE_WRITE);
        autorun.write(";", 1);
        autorun.write(it->first.c_str(), it->first.length());
        autorun.write(";", 1);
        autorun.close();        
        lastbutton = pimoroni::millis();
      }
    }

    if (button_x.raw()) {
      if ((size_t) selected == (commands.size() - 1)) {
        selected = -1;
        draw_output(true);
      }
      else if (selected == -1) {
        selected = 0;
        draw_output(false);
      }
      else
        selected++;
      draw_commands();
      lastbutton = pimoroni::millis();
    }

    if (button_y.raw()) {
      if (selected != -1) {
        auto it = commands.begin();
        std::advance(it, selected);
        // make sure none of your custom commands end in .dd! 
        if (it->first.substr(it->first.length() - 3) == ".dd") {
          selected_duck = it->first;
          run_duck();
        }
        else {
          it->second();
        }
        draw_commands();
        lastbutton = pimoroni::millis();
      }
    }
  }
  /* 
  #elif USB_DISPLAY == CUSTOM_DISPLAY
  // define your button / loop code here
  */
  #endif
}

// standard display functions
void draw_blank_screen(void) {
  #if USB_DISPLAY == NO_DISPLAY
  #else
    graphics.clear();
    graphics.set_pen(BLACK);
    graphics.rectangle(Rect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT));
    display.update(&graphics);
  #endif
}
void draw_navbar(void) {
  #if USB_DISPLAY == NO_DISPLAY
  #else
    graphics.set_pen(COLOUR);
    graphics.rectangle(Rect(0, 0, DISPLAY_WIDTH, int(DISPLAY_HEIGHT / 8)));
    graphics.set_pen(BLACK);
    graphics.text("PICOUSB", Point(5, 5), 50, 3);
    graphics.set_pen(WHITE);
    graphics.rectangle(Rect(120, 5, 3, 19));
    graphics.set_pen(BLACK);
    std::string message;
    switch (wifi_status)
    {
      case WIFI_CHIP_ERROR:
        message = "ERROR";
        break;
      case WIFI_FAILED:
        message = "FAILED";
        break;
      case WIFI_CONNECTED:
        message = ip_address;
        break;
      case WIFI_CONNECTING:
        message = "CONNECTING";
        break;
      case WIFI_NOT_CONNECTED:
        message = "NOT CONNECTED";
        break;
      default:
        print("unknown wifi status");
        message = "uknown";
    }
    graphics.text("WIFI: " + message, Point(130, 8), 200, 2);
    display.update(&graphics);
  /*
  #elif USB_DISPLAY == CUSTOM_DISPLAY
  // define your navbar code here
  */
  #endif
}
void draw_commands(void) {
  #if USB_DISPLAY == NO_DISPLAY
  #else
    graphics.set_pen(WHITE);
    graphics.text("COMMANDS", Point(5, 35), DISPLAY_WIDTH);
    
    int i = 0;
    int offset = 0;
    for (auto const& [key, val] : commands) {
      if (i == selected) {
        graphics.set_pen(COLOUR);
      } 
      else {
        graphics.set_pen(WHITE);
      }
      graphics.text(key, Point(5, 60 + ((i + offset) * 15)), 100);
      if (key.length() > 10)
        offset++;
      i++;
    }
      display.update(&graphics);
  #endif
}
void draw_output(bool selected) {
  #if USB_DISPLAY == NO_DISPLAY
  #else
    graphics.set_pen(BLACK);
    graphics.rectangle(Rect(150, 30, 210, 210));
    graphics.set_pen(WHITE);
    graphics.rectangle(Rect(150, 50, 1, 180));
    graphics.text("OUTPUT", Point(155, 35), DISPLAY_WIDTH);
    graphics.set_pen(selected ? COLOUR : WHITE);
    graphics.text(output, Point(155, scroll), 150, 1);
    draw_navbar(); // navbar will update the graphics
  #endif
}

// standard output functions
void print(const char* text) {
  output += text;
  output += "\n";
  draw_output();
}
void println(const char* text) {
  output += text;
  output += "\n\n";
  draw_output();
}

// msc functions
void format_fat12(void) {
  uint8_t workbuf[4096];
  FATFS elmchamFatfs;
  FRESULT r = f_mkfs("", FM_FAT, 0, workbuf, sizeof(workbuf));
  if (r != FR_OK)
    while (1) yield();

  r = f_mount(&elmchamFatfs, "0:", 1);
  if (r != FR_OK)
    while (1) yield();

  r = f_setlabel(DISK_LABEL);
  if (r != FR_OK)
    while (1) yield();

  f_unmount("0:");
  flash.syncBlocks();
}
int32_t msc_read_cb(uint32_t lba, void *buffer, uint32_t bufsize) {
  return flash.readBlocks(lba, (uint8_t *)buffer, bufsize / 512) ? bufsize : -1;
}
int32_t msc_write_cb(uint32_t lba, uint8_t *buffer, uint32_t bufsize) {
  return flash.writeBlocks(lba, buffer, bufsize / 512) ? bufsize : -1;
}
void msc_flush_cb(void) {
  flash.syncBlocks();
  fatfs.cacheClear();
}
