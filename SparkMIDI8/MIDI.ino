#include "MIDI.h"


void setup_midi() {
  byte b;
  
  if (Usb.Init() == -1) {
    DEBUG("USB host init failed");
    while (1); //halt
  }
  DEBUG("USB host running");

  ser = new HardwareSerial(2); 
  ser->begin(31250, SERIAL_8N1, 16, -1);

  while (ser->available())
    b = ser->read();

  ble_midi_state = 0;
  
}

bool update_midi(byte *mid) {
  bool got_midi;
  byte b;
  
  got_midi = false;

  // process MIDI - both USB and BLE and  Serial DIN

#ifdef BLE_MIDI_ON
  if (!ble_midi_in.is_empty()) {
    ble_midi_in.get(&b);
    if (b & 0x80) {
      mid[0] = b;
      ble_midi_state = 1;
    }
    else {
      if (ble_midi_state == 1) {
        mid[1] = b;
        ble_midi_state++;
      } 
      else {
        if (ble_midi_state == 2) {
          mid[2] = b;
          ble_midi_state = 0;
          got_midi = true;
        }
      }
    }
  }
#endif

  // Serial DIN MIDI
  if (ser->available()) {
    mid[0] = ser->read();
    mid[1] = ser->read();
    mid[2] = ser->read();
    if (mid[0] != 0xFE) {
      got_midi = true;
      set_conn_received(SER_MIDI);
    }
  }
  
  // USB MIDI  
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
  
  // BLE MIDI
  if (!midi_in.is_empty()) {                                   // Bluetooth Midi
    midi_in.get(&mid[0]);  // junk, discard
    midi_in.get(&mid[0]);  // junk, discard
    midi_in.get(&mid[0]);    
    midi_in.get(&mid[1]);
    midi_in.get(&mid[2]);
    got_midi = true;
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
