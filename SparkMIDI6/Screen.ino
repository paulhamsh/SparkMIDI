
#include "Screen.h"

unsigned long now, last_time;

void setup_screen() {
  oled.clear();
  oled.setColor(WHITE);
  oled.drawRect(0, 0, 45, 12);  
#ifdef CLASSIC
  oled.drawString(2,0, "Classic");
#else
  oled.drawString(2,0, "NimBLE");
#endif

  for (int i = 0; i < NUM_CONNS ; i++) {
    oled.drawCircle(CON_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD);
    oled.drawCircle(TO_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD);
    oled.drawCircle(FROM_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD);
  }
  
  oled.drawString(CON_COL-10, TITLES, "Con  To  Frm"); 
  oled.drawString(0, SPK_TEXT, "Spark"); 
  oled.drawString(0, APP_TEXT, "App"); 
  oled.drawString(0, BLE_MIDI_TEXT, "BLE MIDI"); 
  oled.drawString(0, USB_MIDI_TEXT, "USB MIDI");
  oled.drawString(0, SER_MIDI_TEXT, "SER MIDI");
  oled.display();
  
  now = millis();
  last_time = millis();
}



void show_status() {
  int rad;

  if (millis() - now > 50) { // only do this evey 50ms so we don't keep redrawing!
    now = millis();
  
    for (int i = 0; i <= 1; i++) 
      for (int j = 0; j < NUM_CONNS; j++) 
        if (now - conn_last_changed[i][j] <= 900) {
          rad = FILL_RAD - int(FILL_RAD * (now - conn_last_changed[i][j]) / 800); 
          oled.setColor(BLACK);
          oled.fillCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD);
          oled.setColor(WHITE);
          oled.drawCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD);  
          if (rad > 0) {
            oled.fillCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, rad);  
            }
          oled.display();
        }

    for (int j = 0; j <= 2; j++) 
      if (conn_last_changed[STATUS][j] >= last_time)
        if (conn_status[j]) {
          oled.setColor(WHITE);
          oled.fillCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD);
          oled.display();
        }
        else {
          oled.setColor(BLACK);
          oled.fillCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD);
          oled.setColor(WHITE);
          oled.drawCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD);
          oled.display();
        }
  
    last_time = now;
  }
}
