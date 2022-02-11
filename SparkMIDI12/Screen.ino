#include "Screen.h"

#ifdef CLASSIC
char ble_type[10] = "Classic";
#else
char ble_type[10] = "NimBLE";
#endif

#define SCR_TITLE      "Spark MIDI"
#define SCR_SUBTITLE1  "MIDI control for"
#define SCR_SUBTITLE2  "PG Spark 40"
#define SCR_VER "1.2"

#define SCR_COLS        "Con To Frm" 
#define SCR_COLS_L      " Conn  To  From"
#define SCR_SPARK       "Spark"
#define SCR_APP         "App"
#define SCR_BLE_MIDI    "BLE MIDI"
#define SCR_USB_MIDI    "USB MIDI"
#define SCR_SER_MIDI    "SER MIDI"


unsigned long now, last_time;

void splash_screen() {
#ifdef OLED_ON
  Wire.begin(OLED_SDA, OLED_SCL);  // set my SDA, SCL
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("SSD1306 allocation failed");
    while (true); 
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(5, 0);
  display.print(SCR_TITLE);
  display.setCursor(40, 22);
  display.print(SCR_VER);
  display.setTextSize(1);
  display.setCursor(14, 42); 
  display.print(SCR_SUBTITLE1);
  display.setCursor(24, 52); 
  display.print(SCR_SUBTITLE2); 
  display.display();
#endif

#if defined M5STICK
  M5.Lcd.setRotation(-1);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(5, 0);
  M5.Lcd.print(SCR_TITLE);
  M5.Lcd.setCursor(40, 22);
  M5.Lcd.print(SCR_VER);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(14, 42); 
  M5.Lcd.print(SCR_SUBTITLE1);
  M5.Lcd.setCursor(24, 52); 
  M5.Lcd.print(SCR_SUBTITLE2); 
#endif

#if defined M5CORE2  
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(60, 0);
  M5.Lcd.print(SCR_TITLE);
  M5.Lcd.setCursor(120, 66);
  M5.Lcd.print(SCR_VER);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(52, 126); 
  M5.Lcd.print(SCR_SUBTITLE1);
  M5.Lcd.setCursor(82, 156); 
  M5.Lcd.print(SCR_SUBTITLE2); 
#endif

#if defined HELTEC_WIFI
  Heltec.display->clear();
  Heltec.display->setColor(WHITE);
  Heltec.display->setFont(ArialMT_Plain_24);
  Heltec.display->drawString(5, 0, SCR_TITLE); 
  Heltec.display->drawString(40, 22, SCR_VER); 
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawString(24, 42, SCR_SUBTITLE1); 
  Heltec.display->drawString(30, 52, SCR_SUBTITLE2); 
  Heltec.display->display();
#endif
}


void setup_screen() {
#ifdef OLED_ON
  display.clearDisplay();
  display.drawRect(0, 0, 45, 10, WHITE);  
  display.setCursor(2, 2);
  display.print(ble_type);
  for (int i = 0; i < NUM_CONNS ; i++) {
    display.drawCircle(CON_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD, WHITE);
    display.drawCircle(TO_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD, WHITE);
    display.drawCircle(FROM_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD, WHITE);
  }
  display.setCursor(CON_COL-10, TITLES);
  display.print(SCR_COLS); 
  display.setCursor(0, SPK_TEXT);
  display.print(SCR_SPARK); 
  display.setCursor(0, APP_TEXT);
  display.print(SCR_APP); 
  display.setCursor(0, BLE_MIDI_TEXT); 
  display.print(SCR_BLE_MIDI); 
  display.setCursor(0, USB_MIDI_TEXT);
  display.print(SCR_USB_MIDI);
  display.setCursor(0, SER_MIDI_TEXT);
  display.print(SCR_SER_MIDI);
  display.display();
#endif

#if defined M5STICK
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.drawRect(0, 0, 45, 10, TFT_WHITE);   
  M5.Lcd.setCursor(2, 2);
  M5.Lcd.print(ble_type);
  for (int i = 0; i < NUM_CONNS ; i++) {
    M5.Lcd.drawCircle(CON_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD, TFT_WHITE);
    M5.Lcd.drawCircle(TO_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD, TFT_WHITE);
    M5.Lcd.drawCircle(FROM_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD, TFT_WHITE); 
  }
  M5.Lcd.setCursor(CON_COL-10, TITLES);
  M5.Lcd.print(SCR_COLS); 
  M5.Lcd.setCursor(0, SPK_TEXT);
  M5.Lcd.print(SCR_SPARK); 
  M5.Lcd.setCursor(0, APP_TEXT);
  M5.Lcd.print(SCR_APP); 
  M5.Lcd.setCursor(0, BLE_MIDI_TEXT); 
  M5.Lcd.print(SCR_BLE_MIDI); 
  M5.Lcd.setCursor(0, USB_MIDI_TEXT);
  M5.Lcd.print(SCR_USB_MIDI);
  M5.Lcd.setCursor(0, SER_MIDI_TEXT);
  M5.Lcd.print(SCR_SER_MIDI);   
#endif

#ifdef M5CORE2  
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.fillRect(0, 0, L_BOX_SPLIT, 30, TFT_WHITE);   
  M5.Lcd.drawRect(L_BOX_SPLIT+2, 0, 320-L_BOX_SPLIT-2, 30, TFT_WHITE); 
  M5.Lcd.setCursor(6,6);
  M5.Lcd.setTextColor(TFT_BLACK);
  M5.Lcd.print(title);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setCursor(L_TO_COL, 6);
  M5.Lcd.print(ble_type);
  for (int i = 0; i < NUM_CONNS ; i++) {
    M5.Lcd.drawCircle(L_CON_COL, L_SPK_ICON + L_LINES_GAP * i, L_CIRC_RAD, TFT_WHITE);
    M5.Lcd.drawCircle(L_TO_COL, L_SPK_ICON + L_LINES_GAP * i, L_CIRC_RAD, TFT_WHITE);
    M5.Lcd.drawCircle(L_FROM_COL, L_SPK_ICON + L_LINES_GAP * i, L_CIRC_RAD, TFT_WHITE);  
  }   
  M5.Lcd.setCursor(L_CON_COL-40, L_TITLES);
  M5.Lcd.print(SCR_COLS_L); 
  M5.Lcd.setCursor(0, L_SPK_TEXT);
  M5.Lcd.print(SCR_SPARK); 
  M5.Lcd.setCursor(0, L_APP_TEXT);
  M5.Lcd.print(SCR_APP); 
  M5.Lcd.setCursor(0, L_BLE_MIDI_TEXT); 
  M5.Lcd.print(SCR_BLE_MIDI); 
  M5.Lcd.setCursor(0, L_USB_MIDI_TEXT);
  M5.Lcd.print(SCR_USB_MIDI);
  M5.Lcd.setCursor(0, L_SER_MIDI_TEXT);
  M5.Lcd.print(SCR_SER_MIDI);
#endif

#if defined HELTEC_WIFI
  Heltec.display->clear();
  Heltec.display->setColor(WHITE);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->drawRect(0, 0, 45, 12);  
  Heltec.display->drawString(2,0, ble_type);
  for (int i = 0; i < NUM_CONNS ; i++) {
    Heltec.display->drawCircle(CON_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD);
    Heltec.display->drawCircle(TO_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD);
    Heltec.display->drawCircle(FROM_COL, SPK_ICON + LINES_GAP * i, CIRC_RAD);
  }

  Heltec.display->drawString(CON_COL-10, TITLES, SCR_COLS); 
  Heltec.display->drawString(0, SPK_TEXT, SCR_SPARK); 
  Heltec.display->drawString(0, APP_TEXT, SCR_APP); 
  Heltec.display->drawString(0, BLE_MIDI_TEXT, SCR_BLE_MIDI); 
  Heltec.display->drawString(0, USB_MIDI_TEXT, SCR_USB_MIDI);
  Heltec.display->drawString(0, SER_MIDI_TEXT, SCR_SER_MIDI);
  Heltec.display->display();
#endif

  now = millis();
  last_time = millis();
}



void show_status() {
  int rad;

  if (millis() - now > 50) { // only do this evey 50ms so we don't keep redrawing!
    now = millis();

    #ifdef OLED_ON
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
    #endif

    #ifdef M5STICK
    for (int i = 0; i <= 1; i++) 
      for (int j = 0; j < NUM_CONNS; j++) 
        if (now - conn_last_changed[i][j] <= 900) {
          rad = FILL_RAD - int(FILL_RAD * (now - conn_last_changed[i][j]) / 800); 
          M5.Lcd.fillCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, TFT_BLACK);
          M5.Lcd.drawCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, TFT_WHITE);  
          if (rad > 0) {
            M5.Lcd.fillCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, rad, TFT_WHITE);
          }
        }

    for (int j = 0; j <= 2; j++) 
      if (conn_last_changed[STATUS][j] >= last_time)
        if (conn_status[j]) {
          M5.Lcd.fillCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, TFT_WHITE);      
        }
        else {
          M5.Lcd.fillCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, TFT_BLACK);
          M5.Lcd.drawCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD, TFT_WHITE);
        }
    #endif

    #ifdef M5CORE2
    for (int i = 0; i <= 1; i++) 
      for (int j = 0; j < NUM_CONNS; j++) 
        if (now - conn_last_changed[i][j] <= 900) {
          rad = L_FILL_RAD - int(L_FILL_RAD * (now - conn_last_changed[i][j]) / 800); 
          M5.Lcd.fillCircle(i == TO ? L_TO_COL : L_FROM_COL, L_SPK_ICON + j * L_LINES_GAP, L_CIRC_RAD, TFT_BLACK);
          M5.Lcd.drawCircle(i == TO ? L_TO_COL : L_FROM_COL, L_SPK_ICON + j * L_LINES_GAP, L_CIRC_RAD, TFT_WHITE);  
          if (rad > 0) {
            M5.Lcd.fillCircle(i == TO ? L_TO_COL : L_FROM_COL, L_SPK_ICON + j * L_LINES_GAP, rad, TFT_WHITE);
          }
        }

    for (int j = 0; j <= 2; j++) 
      if (conn_last_changed[STATUS][j] >= last_time)
        if (conn_status[j]) {
          M5.Lcd.fillCircle(L_CON_COL, L_SPK_ICON + j * L_LINES_GAP, L_CIRC_RAD, TFT_WHITE);      
        }
        else {
          M5.Lcd.fillCircle(L_CON_COL, L_SPK_ICON + j * L_LINES_GAP, L_CIRC_RAD, TFT_BLACK);
          M5.Lcd.drawCircle(L_CON_COL, L_SPK_ICON + j * L_LINES_GAP, L_CIRC_RAD, TFT_WHITE);
        }
    #endif

    #ifdef HELTEC_WIFI
    for (int i = 0; i <= 1; i++) 
      for (int j = 0; j < NUM_CONNS; j++) 
        if (now - conn_last_changed[i][j] <= 900) {
          rad = FILL_RAD - int(FILL_RAD * (now - conn_last_changed[i][j]) / 800); 
          Heltec.display->setColor(BLACK);
          Heltec.display->fillCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD);
          Heltec.display->setColor(WHITE);
          Heltec.display->drawCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD);  
          if (rad > 0) {
            Heltec.display->fillCircle(i == TO ? TO_COL : FROM_COL, SPK_ICON + j * LINES_GAP, rad);  
            }
          Heltec.display->display();
        }

    for (int j = 0; j <= 2; j++) 
      if (conn_last_changed[STATUS][j] >= last_time)
        if (conn_status[j]) {
          Heltec.display->setColor(WHITE);
          Heltec.display->fillCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD);
          Heltec.display->display();
        }
        else {
          Heltec.display->setColor(BLACK);
          Heltec.display->fillCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD);
          Heltec.display->setColor(WHITE);
          Heltec.display->drawCircle(CON_COL, SPK_ICON + j * LINES_GAP, CIRC_RAD);
          Heltec.display->display();
        }
    #endif
    
    last_time = now;
  }
}
