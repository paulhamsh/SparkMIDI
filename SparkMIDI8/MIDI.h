#ifndef MIDI_h
#define MIDI_h

#include <usbh_midi.h>
#include <SPI.h>

USB Usb;
USBH_MIDI Midi(&Usb);
HardwareSerial *ser;

int ble_midi_state;
uint16_t rcvd;
uint8_t chan;
uint8_t midi_buf[50*4];

void setup_midi();
bool update_midi(byte *mid);

#endif
