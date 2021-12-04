#ifndef Screen_h
#define Screen_h

#define CON_COL 152
#define TO_COL 214
#define FROM_COL 276

#define CIRC_RAD 12
#define FILL_RAD 10

#define TYPE_OFFSET 46
#define LINES_GAP 30

#define TITLES (TYPE_OFFSET + LINES_GAP + 6)
#define SPK_TEXT (TITLES + LINES_GAP)
#define APP_TEXT (SPK_TEXT + LINES_GAP)
#define BLE_MIDI_TEXT (APP_TEXT + LINES_GAP)
#define USB_MIDI_TEXT (BLE_MIDI_TEXT + LINES_GAP)

#define SPK_ICON (SPK_TEXT + 6)
#define APP_ICON (SPK_ICON + LINES_GAP)
#define BLE_MIDI_ICON (APP_ICON + LINES_GAP)
#define USB_MIDI_ICON (BLE_MIDI_ICON + LINES_GAP)

#define SPK 0
#define APP 1
#define BLE_MIDI 2
#define USB_MIDI 3

#define TO 0
#define FROM 1

unsigned long now;
int flash[2][4] = {{FILL_RAD, FILL_RAD, FILL_RAD, FILL_RAD}, {FILL_RAD, FILL_RAD, FILL_RAD, FILL_RAD}};
bool conn_status[4]={false, false, false, false};
bool conn_changed = false;

void set_connected(int conn);
void set_disconnected(int conn);
void setup_screen();
void set_flash(int to_from, int conn);
void show_status();

#endif
