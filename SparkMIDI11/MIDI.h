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
