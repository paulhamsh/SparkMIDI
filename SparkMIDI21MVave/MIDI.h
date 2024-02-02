#ifndef MIDI_h
#define MIDI_h

#ifdef USB_HOST
#include <usbh_midi.h>
#include <SPI.h>

USB Usb;
USBH_MIDI Midi(&Usb);
bool usb_connected;
uint16_t rcvd;
uint8_t chan;
uint8_t midi_buf[50*4];
#endif

#ifdef USB_TRINKET
HardwareSerial *ser2;
#endif

//
#ifdef HELTEC_WIFI  
  #define SER_RX 34
#endif
#ifdef ESP_DEVKIT
  #define SER_RX 16
#endif
#if defined M5CORE || defined M5CORE2 
  #define SER_RX 16
#endif
#ifdef USB_TRINKET
  #ifdef ESP_DEVKIT
    #define SER_RX 17
    #define SER2_RX 19
    #define SER2_TX 18
  #else  // can only be M5STICK or M5CORE2 
    #define SER_RX 26
    #define SER2_RX 0
    #define SER2_TX -1
  #endif
#endif

HardwareSerial *ser1;

class MIDIState 
{
  public:
    MIDIState() {};
    void initialise(RingBuffer *rb);
    bool process(byte mi_data[3]);

    RingBuffer *midi_stream;
    int midi_status;
    int midi_cmd_count;
    int midi_data_count;
    int midi_data[2];
};


void setup_midi();
bool update_midi(byte *mid);

#endif
