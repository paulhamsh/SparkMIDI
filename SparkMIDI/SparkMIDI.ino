#define M5_CORE

#if defined(M5_CORE2)
#include <M5Core2.h> 
#elif defined(M5_CORE)
#include <M5Stack.h>
#endif

#include "Spark.h"

#define PGM_NAME "SparkUITest 7"

SparkPreset my_preset{0x0,0x7f,"F00DF00D-FEED-0123-4567-987654321004","Paul Preset Test","0.7","Nothing Here","icon.png",120.000000,{ 
  {"bias.noisegate", true, 2, {0.316873, 0.304245}}, 
  {"Compressor", false, 2, {0.341085, 0.665754}}, 
  {"Booster", true, 1, {0.661412}}, 
  {"Bassman", true, 5, {0.768152, 0.491509, 0.476547, 0.284314, 0.389779}}, 
  {"UniVibe", false, 3, {0.500000, 1.000000, 0.700000}}, 
  {"VintageDelay", true, 4, {0.152219, 0.663314, 0.144982, 1.000000}}, 
  {"bias.reverb", true, 7, {0.120109, 0.150000, 0.500000, 0.406755, 0.299253, 0.768478, 0.100000}} },0x00 };

#include <usbh_midi.h>
#include <SPI.h>

USB Usb;
USBH_MIDI Midi(&Usb);

/* MIDI EVENTS

MIDI COMMANDER
--------------

1    2    3    4
A    B    C    D

 3   9   14   29
22  23   24  105

1     CC3  127
2     CC9  127
3     CC14 127
4     CC29 127
A     CC22 127
B     CC23 127
C     CC24 127
D     CC105 127

EXP1  CC1 0 - 127
EXP2  CC2 0 - 127


LPD8
----
P5 P6 P7 P8 K1 K2 K3 K4
P1 P2 P3 P4 K5 K6 K7 K8

31 33 35 36  0  1  2  3
24 26 28 29  4  5  6  7


PAD 1  NOTE ON 24   1-127 / NOTE OFF 0
PAD 2  NOTE ON 26   1-127 / NOTE OFF 0
PAD 3  NOTE ON 28   1-127 / NOTE OFF 0
PAD 4  NOTE ON 29   1-127 / NOTE OFF 0
PAD 5  NOTE ON 31   1-127 / NOTE OFF 0
PAD 6  NOTE ON 33   1-127 / NOTE OFF 0
PAD 7  NOTE ON 35   1-127 / NOTE OFF 0
PAD 8  NOTE ON 36   1-127 / NOTE OFF 0

K1     CC 0 0 - 127
K2     CC 1 0 - 127
K3     CC 2 0 - 127
K4     CC 3 0 - 127
K5     CC 4 0 - 127
K6     CC 5 0 - 127
K7     CC 6 0 - 127
K8     CC 7 0 - 127
*/


// LPD8 gives 0xab 0xcd 0x90 0xNN 0xef or 0xab 0xcd 0x80 0xNN 0x00 for on and off
// NN is 1f 21 23 24
// NN is 18 1a 1c 1d

// Blueboard does this:
// In mode B the BB gives 0x80 0x80 0x90 0xNN 0x64 or 0x80 0x80 0x80 0xNN 0x00 for on and off
// In mode C the BB gives 0x80 0x80 0xB0 0xNN 0x7F or 0x80 0x80 0xB0 0xNN 0x00 for on and off


///// MAIN PROGRAM

unsigned long last_millis;
unsigned long delay_it;
float val, val_inc;
bool onoroff;
bool got_midi;

uint16_t rcvd;
uint8_t chan;
uint8_t midi_buf[50*4];
byte mi1, mi2, mi3;
int midi_chan, midi_cmd;
    
void setup() {
#if defined(M5_CORE2) || defined(M5_CORE)
  M5.begin();
#endif

#if defined(M5_CORE)
  M5.Power.begin();
#endif

#if defined(M5_CORE2) || defined(M5_CORE)
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(4);
  M5.Lcd.println(PGM_NAME);
#else
  Serial.begin(115200);
#endif

  if (Usb.Init() == -1) {
    Serial.println("USB host init failed");
    while (1); //halt
  }
  Serial.println("USB host running");

  Serial.println("Started");

  last_millis = millis();
  delay_it = 60000;
  val = 0.0;  
  val_inc = 0.05;
  onoroff = true;

  spark_state_tracker_start();  // set up data to track Spark and app state

  Serial.println("Started");

  
#if defined(M5_CORE2) || defined(M5_CORE)
  M5.Lcd.println("Running...");
#endif
}


void loop() {

#if defined(M5_CORE2) || defined(M5_CORE) 
  M5.update();

  if (M5.BtnA.wasPressed()) {
    Serial.println("BUTTON A PRESS");
    change_amp_model("94MatchDCV2");
    change_drive_model("Booster");  
    change_mod_model("GuitarEQ6");
    change_delay_model("DelayMono");
    update_ui();
  }

  if (M5.BtnB.wasPressed()) {
    Serial.println("BUTTON B PRESS");
    change_amp_model("Twin");
    change_drive_model("MaestroBassmaster");
    change_mod_model("Tremolo");
    change_mod_onoff(false);
    update_ui();
  }    

  if (M5.BtnC.wasPressed()) {
    Serial.println("BUTTON C PRESS");
    change_custom_preset(&my_preset, 4);
    update_ui();
  }    
#endif
  got_midi = false;
  Usb.Task();

  if (Midi) {                                                  // USB Midi
    rcvd = Midi.RecvData(midi_buf, false);
    if (rcvd > 0 && !(midi_buf[0] == 0 && midi_buf[1] == 0 && midi_buf[2] == 0)) {
      got_midi = true;
      mi1 = midi_buf[0];
      mi2 = midi_buf[1];
      mi3 = midi_buf[2];
    }
  }

  if (!midi_in.is_empty()) {                                   // Bluetooth Midi
    midi_in.get(&mi1);  // junk, discard
    midi_in.get(&mi1);  // junk, discard
    midi_in.get(&mi1);    
    midi_in.get(&mi2);
    midi_in.get(&mi3);
    got_midi = true;
  }

  if (got_midi) {
    midi_chan = (mi1 & 0x0f) + 1;
    midi_cmd = mi1 & 0xf0;
    
    Serial.print(mi1, HEX); Serial.print(" "); Serial.print (midi_chan); Serial.print(" ");
    Serial.print(midi_cmd, HEX); Serial.print(" "); Serial.print(mi2); Serial.print(" "); Serial.println(mi3);
    
    if (mi1 == 0x90) {
      switch (mi2) {
        case 0x1f:  // pad 5
          change_drive_toggle();
          break;        
        case 0x21:  // pad 6
          change_mod_toggle();
          break;        
        case 0x23:  // pad 7
          change_delay_toggle();
          break;
        case 0x24:  // pad 8
          change_reverb_toggle();
          break;
        case 0x18:  // pad 1
          change_hardware_preset(0);
          break;
        case 0x1a:  // pad 2
          change_hardware_preset(1);
          break;
        case 0x1c:  // pad 3
          change_hardware_preset(2);        
          break;
        case 0x1d:  // pad 4
          change_hardware_preset(3);        
          break;
      }
    }

    if (mi1 == 0xb0) {
      switch (mi2) {
        case 0:  // k1
          change_amp_param(AMP_GAIN, mi3/127.0);
          break;        
        case 1:  // k2
          change_amp_param(AMP_BASS, mi3/127.0);
          break;        
        case 2:  // k3
          change_amp_param(AMP_MID, mi3/127.0);
          break;
        case 3:  // k4
          change_amp_param(AMP_TREBLE, mi3/127.0);
          break;
        case 4:  // k5
          change_amp_param(AMP_MASTER, mi3/127.0);
          break;
        case 5:  // k6
          break;
        case 6:  // k7
          break;
        case 7:  // k8
          break;
      }
    }
  
    if (mi1 == 0xb3) {               // chan 4 is MIDI Commander
      switch (mi2) {
        case 1:  // expression pedal l
          change_amp_param(AMP_GAIN, mi3/127.0);
          break;   
        case 2:  // expression pedal 2
          change_amp_param(AMP_MASTER, mi3/127.0);
          break;   
        case 3:  // pad 5
          change_drive_toggle();
          break;        
        case 9:  // pad 6
          change_mod_toggle();
          break;        
        case 14:  // pad 7
          change_delay_toggle();
          break;
        case 29:  // pad 8
          change_reverb_toggle();
          break;
        case 22:  // pad 1
          change_hardware_preset(0);
          break;
        case 23:  // pad 2
          change_hardware_preset(1);
          break;
        case 24:  // pad 3
          change_hardware_preset(2);        
          break;
        case 105:  // pad 4
          change_hardware_preset(3);        
          break;
        }
    }
  }

  if (update_spark_state()) {
    // do your own checks and processing here    
  }
}
