/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// DISPLAY CONFIG
#define NO_DISPLAY 0
#define PICO_DISPLAY_2 1
//#define CUSTOM_DISPLAY 2

#define USB_DISPLAY PICO_DISPLAY_2

#if USB_DISPLAY == NO_DISPLAY
  #define DISPLAY_WIDTH 0
  #define DISPLAY_HEIGHT 0
#elif USB_DISPLAY == PICO_DISPLAY_2
  #define DISPLAY_WIDTH 320
  #define DISPLAY_HEIGHT 240
/*
#elif USB_DISPLAY == CUSTOM_DISPLAY
  #define DISPLAY_WIDTH 320
  #define DISPLAY_HEIGHT 240
*/
#endif

// OTHER CONFIG
#define AUTORUN_DELAY 2000 // host computer takes a few seconds to register drive
#define DEFAULT_DELAY 200

#define YOUR_SSID "YOUR_SSID" // SSID is taken by Arduino somewhere
#define YOUR_PASS "YOUR_PASSWORD"

// please note CD_TO_DRIVE UK/US (only difference in this case is " and @, however if you are UK (or not US) then you'll have to mess around with 
// the correct keys since I haven't added support for keyboard layouts yet (outside of US) - I have included UK example because I'm UK :)
// another messy character is Pipe character, I could not get this to work without doing ALT+124 which is a pain. SORRY! Will work on this in future.
#define DISK_LABEL "picousb"
#define CD_TO_DRIVE Keyboard.println("for /f @tokens=1 delims==:@ %i in ('wmic logicaldisk where @volumename=@picousb@@ get deviceid') do (%i:)") //this is for UK host
//#define CD_TO_DRIVE Keyboard.println("for /f \"tokens=1 delims==:\" %i in ('wmic logicaldisk where \"volumename=\"picousb\"\" get deviceid') do (%i:)") // this is for US host
#define DEBOUNCE 200

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if USB_DISPLAY == PICO_DISPLAY_2
  #include "src/pimoroni-pico/libraries/pico_display_2/pico_display_2.hpp"
  #include "src/pimoroni-pico/drivers/st7789/st7789.hpp"
  #include "src/pimoroni-pico/libraries/pico_graphics/pico_graphics.hpp"
  #include "src/pimoroni-pico/drivers/button/button.hpp"
/*
#elif DISPLAY == CUSTOM_DISPLAY
  #include "src/libraries/custom_display/custom_display.hpp"
*/
#endif

#if USB_DISPLAY == PICO_DISPLAY_2
  using namespace pimoroni;
  ST7789 display(320, 240, ROTATE_0, false, get_spi_pins(BG_SPI_FRONT));
  PicoGraphics_PenRGB332 graphics(display.width, display.height, nullptr);
  Pen BLACK;
  Pen WHITE;
  Pen COLOUR;
  Button button_a(PicoDisplay2::A);
  Button button_b(PicoDisplay2::B);
  Button button_x(PicoDisplay2::X);
  Button button_y(PicoDisplay2::Y);
/*
#elif DISPLAY == CUSTOM_DISPLAY
*/
#endif