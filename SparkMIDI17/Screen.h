#ifndef Screen_h
#define Screen_h

#ifdef OLED_ON
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64, &Wire, -1);
#endif

// OLED settings are different depending on whether a Trinket or Host is used
#ifdef OLED_ON
  #ifdef ESP_DEVKIT
    #ifdef USB_TRINKET
      #define OLED_SDA 16
        #define OLED_SCL 4
     #endif
     #ifdef USB_HOST
       #define OLED_SDA 4
       #define OLED_SCL 15
     #endif
  #endif

  #if defined M5CORE2 || defined M5STICK || defined M5CORE
    #define OLED_SDA 33
    #define OLED_SCL 32
  #endif

  #if defined M5CORE
    #define OLED_SDA 21
    #define OLED_SCL 22
  #endif 
#endif

// Smaller screen
#define CON_COL 70
#define TO_COL 90
#define FROM_COL 110

#define CIRC_RAD 5
#define FILL_RAD 5

#define LINES_GAP 10
#define TITLES 2
#define SPK_TEXT (TITLES + LINES_GAP)
#define APP_TEXT (SPK_TEXT + LINES_GAP)
#define BLE_MIDI_TEXT (APP_TEXT + LINES_GAP)
#define USB_MIDI_TEXT (BLE_MIDI_TEXT + LINES_GAP)
#define SER_MIDI_TEXT (USB_MIDI_TEXT + LINES_GAP)

#define SPK_ICON (SPK_TEXT + 6)
#define APP_ICON (SPK_ICON + LINES_GAP)
#define BLE_MIDI_ICON (APP_ICON + LINES_GAP)
#define USB_MIDI_ICON (BLE_MIDI_ICON + LINES_GAP)
#define SER_MIDI_ICON (USB_MIDI_ICON + LINES_GAP)

// Larger screen

#define L_CON_COL 152
#define L_TO_COL 212
#define L_FROM_COL 272

#define L_BOX_SPLIT 180 

#define L_CIRC_RAD 12
#define L_FILL_RAD 10

#define L_LINES_GAP 30
#define L_TITLES 42
#define L_SPK_TEXT (L_TITLES + L_LINES_GAP)
#define L_APP_TEXT (L_SPK_TEXT + L_LINES_GAP)
#define L_BLE_MIDI_TEXT (L_APP_TEXT + L_LINES_GAP)
#define L_USB_MIDI_TEXT (L_BLE_MIDI_TEXT + L_LINES_GAP)
#define L_SER_MIDI_TEXT (L_USB_MIDI_TEXT + L_LINES_GAP)
#define L_LAST_COMMAND_TEXT (L_SER_MIDI_TEXT + L_LINES_GAP)

#define L_SPK_ICON (L_SPK_TEXT + 6)
#define L_APP_ICON (L_SPK_ICON + L_LINES_GAP)
#define L_BLE_MIDI_ICON (L_APP_ICON + L_LINES_GAP)
#define L_USB_MIDI_ICON (L_BLE_MIDI_ICON + L_LINES_GAP)
#define L_SER_MIDI_ICON (L_USB_MIDI_ICON + L_LINES_GAP)

void splash_screen();
void setup_screen();
void show_status();

#endif
