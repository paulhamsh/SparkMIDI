#ifndef MIDI_h
#define MIDI_h

#include <usbh_midi.h>
#include <SPI.h>

HardwareSerial *ser;
HardwareSerial *ser2;

USB Usb;
USBH_MIDI Midi(&Usb);
bool usb_connected;

int ble_app_status;
int ble_app_cmd_count;
int ble_app_data_count;
int ble_app_data[2];

int ble_mid_status;
int ble_mid_cmd_count;
int ble_mid_data_count;
int ble_mid_data[2];

void setup_midi();
bool update_midi(byte *mid);

#endif
