// ducky script interpreter 
// commands taken from here https://docs.hak5.org/hak5-usb-rubber-ducky/duckyscript-tm-quick-reference

// The Button / LED / ATTACKMODE / Mouse / Logical operations not yet supported!
// If you add more commands, please do below and let me know so I can add them to the main repo!

void run_duck() {
  TCHAR path[FF_MAX_LFN + 1];
  strcpy(path, "ducks/");
  strcat(path, selected_duck.c_str());
  println((const char*) path);
  
  File32 file = fatfs.open(path);
  if (!file) {
    println("Error opening duck");
    return;
  }

  int length = file.available();
  char contents[length+1] = {'\0'};
  std::string contents_str = contents;
  std::string line;
  size_t start = 0;
  size_t end = 0;

  file.read(contents, length);
  file.close();

  size_t npos = (size_t) (-1);
  while(end != npos) {
    end = contents_str.find('\n', start);
    line = contents_str.substr(start, end - start);
    start = end + 1;

    // get command (first word)
    size_t space = line.find(' ', 0);
    if (space == npos) {
      space = line.length();
    }

    const char* cmd = line.substr(0, space).c_str();
    println(cmd);

    // would obviously be faster to use a map but this is nicer to read for now :)
    // comments
    if (!strcmp(cmd, "REM")) { 
      // ignore
    }

    // keystroke injection
    else if (!strcmp(cmd, "STRING")) {
      std::string string_str = line.substr(7);
      Keyboard.print(string_str.c_str());
    }

    else if (!strcmp(cmd, "STRINGLN")) {
      std::string string_str = line.substr(9);
      Keyboard.println(string_str.c_str());
    }
    
    // cursor keys
    else if (!strcmp(cmd, "UP")) {} // UP/DOWN/LEFT/RIGHT (mouse) not supported yet
    else if (!strcmp(cmd, "DOWN")) {}
    else if (!strcmp(cmd, "LEFT")) {}
    else if (!strcmp(cmd, "RIGHT")) {}

    else if (!strcmp(cmd, "UPARROW")) {
      Keyboard.press(KEY_UP_ARROW);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "DOWNARROW")) {
      Keyboard.press(KEY_DOWN_ARROW);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "LEFTARROW")) {
      Keyboard.press(KEY_LEFT_ARROW);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "RIGHTARROW")) {
      Keyboard.press(KEY_RIGHT_ARROW);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "PAGEUP")) {
      Keyboard.press(KEY_PAGE_UP);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "PAGEDOWN")) {
      Keyboard.press(KEY_PAGE_DOWN);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "HOME")) {
      Keyboard.press(KEY_HOME);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "END")) {
      Keyboard.press(KEY_END);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "INSERT")) {
      Keyboard.press(KEY_INSERT);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "DELETE")) {
      Keyboard.press(KEY_DELETE);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "DEL")) {
      Keyboard.press(KEY_RIGHT_ARROW);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
      delay(DEFAULT_DELAY);
      Keyboard.press(KEY_DELETE);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

     else if (!strcmp(cmd, "TAB")) {
      Keyboard.print("\t");
    }

    else if (!strcmp(cmd, "SPACE")) {
      Keyboard.print(" ");
    }

    // system keys
    else if (!strcmp(cmd, "ENTER")) {
      Keyboard.println();
    }

    else if (!strcmp(cmd, "ESCAPE")) {
      Keyboard.press(KEY_ESC);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "PAUSE") || !strcmp(cmd, "BREAK")) { }
    else if (!strcmp(cmd, "PRINTSCREEN")) { }
    else if (!strcmp(cmd, "MENU")) { }
    else if (!strcmp(cmd, "APP")) { }
    
    else if (!strcmp(cmd, "F1")) {
      Keyboard.press(KEY_F1);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "F2")) {
      Keyboard.press(KEY_F2);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "F3")) {
      Keyboard.press(KEY_F3);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "F4")) {
      Keyboard.press(KEY_F4);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "F5")) {
      Keyboard.press(KEY_F5);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "F6")) {
      Keyboard.press(KEY_F6);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "F7")) {
      Keyboard.press(KEY_F7);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "F8")) {
      Keyboard.press(KEY_F8);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "F9")) {
      Keyboard.press(KEY_F9);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "F10")) {
      Keyboard.press(KEY_F10);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "F11")) {
      Keyboard.press(KEY_F11);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "F12")) {
      Keyboard.press(KEY_F12);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    // basic modifier keys
    else if (!strcmp(cmd, "SHIFT")) {
      char key = line[6];
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(key);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "ALT")) {
      char key = line[4];
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(key);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }
    
    else if (!strcmp(cmd, "CONTROL")) {
      char key = line[8];
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(key);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "CTRL")) {
      char key = line[5];
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(key);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "COMMAND")) { }

    else if (!strcmp(cmd, "WINDOWS")) { }

    else if (!strcmp(cmd, "GUI")) {
      char key = line[4];
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(key);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    // advanced mod and standalone mod keys skipped!

    // lock keys
    else if (!strcmp(cmd, "CAPSLOCK")) {
      Keyboard.press(KEY_CAPS_LOCK);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "NUMLOCK")) {
      Keyboard.press(HID_KEY_NUM_LOCK);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    else if (!strcmp(cmd, "SCROLLLOCK")) {
      Keyboard.press(HID_KEY_SCROLL_LOCK);
      delay(DEFAULT_DELAY);
      Keyboard.releaseAll();
    }

    // delays
    else if (!strcmp(cmd, "DELAY")) {
      std::string delay_str = line.substr(6);
      int delay_int = std::stoi(delay_str);
      delay(delay_int);
    }

    else {
      print("UNKNOWN DUCK COMMAND :(");
      println(line.c_str());
    }
  }
}
