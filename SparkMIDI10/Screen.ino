
#include "Screen.h"

unsigned long now, last_time;

void splash_screen() {
  Wire.begin(OLED_SDA, OLED_SCL);  // set my SDA, SCL
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("SSD1306 allocation failed");
    while (true); 
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(5, 0);
  display.print("Spark MIDI");
  display.setCursor(40, 22);
  display.print("0.8");
  display.setTextSize(1);
  display.setCursor(14, 42); 
  display.print("MIDI control for");
  display.setCursor(24, 52); 
  display.print("PG Spark 40"); 
  display.display();
}



void setup_screen() {
  display.clearDisplay();
  display.drawRect(0, 0, 45, 10, WHITE);  
#ifdef CLASSIC
  display.setCursor(2, 2);
  display.print("Classic");
#else
  display.setCursor(2, 2);
  display.print("NimBLE");
#endif

  for (int i = 0; i < NUM_CONNS ; i++) {
    display.drawCircle(CON_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD, WHITE);
    display.drawCircle(TO_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD, WHITE);
    display.drawCircle(FROM_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD, WHITE);
  }
  
  display.setCursor(CON_COL-10, TITLES);
  display.print("Con To Frm"); 
  display.setCursor(0, SPK_TEXT);
  display.print("Spark"); 
  display.setCursor(0, APP_TEXT);
  display.print("App"); 
  display.setCursor(0, BLE_MIDI_TEXT); 
  display.print("BLE MIDI"); 
  display.setCursor(0, USB_MIDI_TEXT);
  display.print("USB MIDI");
  display.setCursor(0, SER_MIDI_TEXT);
  display.print("SER MIDI");
  display.display();
  
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
          display.fillCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, BLACK);
          display.drawCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, WHITE);  
          if (rad > 0) {
            display.fillCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, rad, WHITE); 
            }
          display.display();
        }

    for (int j = 0; j <= 2; j++) 
      if (conn_last_changed[STATUS][j] >= last_time)
        if (conn_status[j]) {
          display.fillCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, WHITE);
          display.display();
        }
        else {
          display.fillCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, BLACK);
          display.drawCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, WHITE);
          display.display();
        }
  
    last_time = now;
  }
}
