#include "MIDI.h"


void setup_midi() {
  byte b;

  if (Usb.Init() == -1) {
    DEBUG("USB host init failed");
    usb_connected = false;
  }
  else {
    DEBUG("USB host running");
    usb_connected = true;   
  }
  
  ser = new HardwareSerial(1); 
  ser->begin(31250, SERIAL_8N1, 17, -1);

  while (ser->available())
    b = ser->read();

  ser2 = new HardwareSerial(2); 
  ser2->begin(115200, SERIAL_8N1, 19, 18);

  while (ser2->available())    b = ser2->read();

  ble_app_data_count = 0;
  ble_app_cmd_count = 0;
  ble_app_status = 0;

  ble_mid_data_count = 0;
  ble_mid_cmd_count = 0;
  ble_mid_status = 0;
}

bool update_midi(byte *mid) {
  bool got_midi;
  byte b;
  
  got_midi = false;

  // process MIDI - both USB and BLE and  Serial DIN

#ifdef BLE_MIDI_ON
  if (!ble_midi_in.is_empty()) {
    ble_midi_in.get(&b);
    
    if (b <= 0x7F) {
      ble_app_data[ble_app_data_count] = b;
      ble_app_data_count++;
      ble_app_cmd_count = 0;
      if ((ble_app_status == 0xC0 || ble_app_status == 0xD0) || ble_app_data_count >= 2) {
        mid[0] = ble_app_status;
        mid[1] = ble_app_data[0];
        if (ble_app_data_count == 2)
          mid[2] = ble_app_data[1];
        else
          mid[2] = 0;
        ble_app_data_count = 0;
        got_midi = true;
      }
    } 
    else {
      ble_app_cmd_count++;
      if (ble_app_cmd_count > 1) {
        ble_app_status = b;            // the last byte before a non-cmd byte will always be status unless it was a single timestamp
      }
    }
  }
#endif

  // Serial DIN MIDI
  if (ser->available()) {
    mid[0] = ser->read();
    mid[1] = ser->read();
    
    if (mid[0] == 0xC0 || mid[0] == 0xD0)
      mid[2] = 0;
    else 
      mid[2] = ser->read();
      
    if (mid[0] != 0xFE) {
      got_midi = true;
      set_conn_received(SER_MIDI);
    }
  }


  // Serial 2 DIN MIDI
  if (ser2->available()) {
    mid[0] = ser2->read();
    mid[1] = ser2->read();
    
    if (mid[0] == 0xC0 || mid[0] == 0xD0)
      mid[2] = 0;
    else 
      mid[2] = ser2->read();
      
    if (mid[0] != 0xFE) {
      got_midi = true;
      set_conn_received(SER_MIDI);
    }
  }
   
  // BLE MIDI controller
  if (!midi_in.is_empty()) {     // Bluetooth Midi
    midi_in.get(&b);
        
    if (b <= 0x7F) {
      ble_mid_data[ble_mid_data_count] = b;
      ble_mid_data_count++;
      ble_mid_cmd_count = 0;
      if ((ble_mid_status == 0xC0 || ble_mid_status == 0xD0) || ble_mid_data_count >= 2) {
        mid[0] = ble_mid_status;
        mid[1] = ble_mid_data[0];
        if (ble_mid_data_count == 2)
          mid[2] = ble_mid_data[1];
        else
          mid[2] = 0;
        ble_mid_data_count = 0;
        got_midi = true;
      }
    } 
    else {
      ble_mid_cmd_count++;
      if (ble_mid_cmd_count > 1) {
        ble_mid_status = b;            // the last byte before a non-cmd byte will always be status unless it was a single timestamp
      }
    }
  }


  // USB MIDI  
  if (usb_connected) {
    Usb.Task();

    if (Midi) {                                                  // USB Midi
      rcvd = Midi.RecvData(midi_buf, false);
      if (rcvd > 0 && !(midi_buf[0] == 0 && midi_buf[1] == 0 && midi_buf[2] == 0)) {
        set_conn_received(USB_MIDI);
        mid[0] = midi_buf[0];
        mid[1] = midi_buf[1];
        mid[2] = midi_buf[2];
        got_midi = true;
      }
    }
  }

  if (got_midi) {
    Serial.print("MIDI ");
    Serial.print(mid[0], HEX);
    Serial.print(" ");
    Serial.print(mid[1]);
    Serial.print(" ");   
    Serial.println(mid[2]);
  }
  return got_midi;
}
