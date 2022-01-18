#ifndef MIDI_h
#define MIDI_h

#ifdef USB_HOST
#include <usbh_midi.h>
#include <SPI.h>
#define SER_RX 16

USB Usb;
USBH_MIDI Midi(&Usb);
bool usb_connected;
uint16_t rcvd;
uint8_t chan;
uint8_t midi_buf[50*4];
#endif

#ifdef USB_TRINKET
HardwareSerial *ser2;
#define SER_RX 17
#define SER2_RX 19
#define SER2_TX 18
#endif

HardwareSerial *ser1;

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
