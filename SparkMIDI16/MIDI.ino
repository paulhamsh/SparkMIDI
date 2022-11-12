#include "MIDI.h"

// MIDI State processing for BLE streams

void MIDIState::initialise(RingBuffer *rb)
{
  midi_stream = rb;
  midi_data_count = 0;
  midi_cmd_count = 0;
  midi_status = 0;
}

bool MIDIState::process(byte mi_data[3]) 
{
  byte b;
  bool got_data;

  got_data = false;
  
  if (!midi_stream->is_empty()) {
    midi_stream->get(&b);
    
    if (b <= 0x7F) {
      midi_data[midi_data_count] = b;
      midi_data_count++;
      midi_cmd_count = 0;
      if ((midi_status == 0xC0 || midi_status == 0xD0) || midi_data_count >= 2) {
        mi_data[0] = midi_status;
        mi_data[1] = midi_data[0];
        if (midi_data_count == 2)
          mi_data[2] = midi_data[1];
        else
          mi_data[2] = 0;
        midi_data_count = 0;
        got_data = true;
      }
    } 
    else {
      midi_cmd_count++;
      if (midi_cmd_count > 1) {
        midi_status = b;            // the last byte before a non-cmd byte will always be status unless it was a single timestamp
      }
    }
  } 
  return got_data;
}

MIDIState ble_app;
MIDIState ble_midi;

void setup_midi() {
  byte b;

#ifdef USB_HOST
  if (Usb.Init() == -1) {
    DEBUG("USB host init failed");
    usb_connected = false;
  }
  else {
    DEBUG("USB host running");
    usb_connected = true;   
  }
#endif
  
  ser1 = new HardwareSerial(1); 
  ser1->begin(31250, SERIAL_8N1, SER_RX, -1);
  while (ser1->available())
    b = ser1->read();

#ifdef USB_TRINKET
  ser2 = new HardwareSerial(2); 
  ser2->begin(115200, SERIAL_8N1, SER2_RX, SER2_TX);
  while (ser2->available())    
    b = ser2->read();
#endif

  ble_app.initialise(&ble_midi_in);
  ble_midi.initialise(&midi_in);
}

bool update_midi(byte *mid) {
  bool got_midi;
  byte b;
  
  got_midi = false;

  // process MIDI - both USB and BLE and  Serial DIN

#ifdef BLE_APP_MIDI
  if (ble_app.process(mid)) got_midi = true;
#endif

  // Serial 1 - DIN MIDI
  if (ser1->available()) {
    mid[0] = ser1->read();
    mid[1] = ser1->read();
    
    if (mid[0] == 0xC0 || mid[0] == 0xD0)
      mid[2] = 0;
    else 
      mid[2] = ser1->read();
      
    if (mid[0] != 0xFE) {
      got_midi = true;
      set_conn_received(SER_MIDI);
    }
  }


#ifdef USB_TRINKET
  // Serial 2 - Trinket M0 USB
  if (ser2->available()) {
    mid[0] = ser2->read();
    mid[1] = ser2->read();
    
    if (mid[0] == 0xC0 || mid[0] == 0xD0)
      mid[2] = 0;
    else 
      mid[2] = ser2->read();
      
    if (mid[0] != 0xFE) {
      got_midi = true;
      set_conn_received(USB_MIDI);
    }
  }
#endif

#ifdef BLE_CONTROLLER
  // BLE MIDI controller
  if (ble_midi.process(mid)) got_midi = true;
#endif

#ifdef USB_HOST
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
#endif

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
