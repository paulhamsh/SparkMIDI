#ifndef Screen_h
#define Screen_h

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128, 64, &Wire, -1);

#define CON_COL 70
#define TO_COL 90
#define FROM_COL 110

#define CIRC_RAD 5
#define FILL_RAD 5

#define LINES_GAP 10

#define TITLES 1
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

void splash_screen();
void setup_screen();
void show_status();

#endif
