#include <vector>
#include <unordered_map>
#include <functional>

#include <string.h>
#include <math.h>
#include <WiFi.h>
#include <SPI.h>
#include <LittleFS.h>

#include "SdFat.h"
#include "Adafruit_TinyUSB.h"
#include "Adafruit_SPIFlash.h"

#include "src/diskio.h"
#include "src/ff.h"
#include "src/TinyUSB_Mouse_and_Keyboard.h"
#include "config.hpp" // !!

// function prototypes
void run_duck();
void draw_blank_screen(void);
void draw_navbar(void);
void draw_commands(void);
void draw_output(bool selected = false);
void print(const char* text);
void println(const char* text);

void format_fat12(void);
int32_t msc_read_cb(uint32_t lba, void *buffer, uint32_t bufsize);
int32_t msc_write_cb(uint32_t lba, uint8_t *buffer, uint32_t bufsize);
void msc_flush_cb(void);

// define globals
enum {
  WIFI_NOT_CONNECTED,
  WIFI_CONNECTING,
  WIFI_FAILED,
  WIFI_CHIP_ERROR,
  WIFI_CONNECTED
};

bool fs_formatted;
int selected;
int scroll;
int lastbutton;
int wifi_status;
bool autorun;
std::string ip_address;
std::string output;
std::string selected_duck;
WiFiServer server(9999);
Adafruit_FlashTransport_RP2040 flashTransport;
Adafruit_SPIFlash flash(&flashTransport);
FatVolume fatfs;
Adafruit_USBD_MSC usb_msc;

// fatfs diskio functions
extern "C" {
  DSTATUS disk_status(BYTE pdrv) {
    (void)pdrv;
    return 0;
  }

  DSTATUS disk_initialize(BYTE pdrv) {
    (void)pdrv;
    return 0;
  }

  DRESULT disk_read(
    BYTE pdrv,     // Physical drive nmuber to identify the drive
    BYTE *buff,    // Data buffer to store read data
    DWORD sector,  // Start sector in LBA
    UINT count     // Number of sectors to read
  ) {
    (void)pdrv;
    return flash.readBlocks(sector, buff, count) ? RES_OK : RES_ERROR;
  }

  DRESULT disk_write(
    BYTE pdrv,         // Physical drive nmuber to identify the drive
    const BYTE *buff,  // Data to be written
    DWORD sector,      // Start sector in LBA
    UINT count         // Number of sectors to write
  ) {
    (void)pdrv;
    return flash.writeBlocks(sector, buff, count) ? RES_OK : RES_ERROR;
  }

  DRESULT disk_ioctl(
    BYTE pdrv,  // Physical drive nmuber (0..)
    BYTE cmd,   // Control code
    void *buff  // Buffer to send/receive control data
  ) {
    (void)pdrv;

    switch (cmd) {
      case CTRL_SYNC:
        flash.syncBlocks();
        return RES_OK;

      case GET_SECTOR_COUNT:
        *((DWORD *)buff) = flash.size() / 512;
        return RES_OK;

      case GET_SECTOR_SIZE:
        *((WORD *)buff) = 512;
        return RES_OK;

      case GET_BLOCK_SIZE:
        *((DWORD *)buff) = 8;  // erase block size in units of sector size
        return RES_OK;

      default:
        return RES_PARERR;
    }
  }
}
