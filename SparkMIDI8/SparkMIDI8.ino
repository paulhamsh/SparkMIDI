
//       .----------------.  .----------------.  .----------------.  .----------------.  .----------------.      .----------------.  .----------------.  .----------------.  .----------------. 
//     | .--------------. || .--------------. || .--------------. || .--------------. || .--------------. |    | .--------------. || .--------------. || .--------------. || .--------------. |
//     | |    _______   | || |   ______     | || |      __      | || |  _______     | || |  ___  ____   | |    | | ____    ____ | || |     _____    | || |  ________    | || |     _____    | |
//     | |   /  ___  |  | || |  |_   __ \   | || |     /  \     | || | |_   __ \    | || | |_  ||_  _|  | |    | ||_   \  /   _|| || |    |_   _|   | || | |_   ___ `.  | || |    |_   _|   | |
//     | |  |  (__ \_|  | || |    | |__) |  | || |    / /\ \    | || |   | |__) |   | || |   | |_/ /    | |    | |  |   \/   |  | || |      | |     | || |   | |   `. \ | || |      | |     | |
//     | |   '.___`-.   | || |    |  ___/   | || |   / ____ \   | || |   |  __ /    | || |   |  __'.    | |    | |  | |\  /| |  | || |      | |     | || |   | |    | | | || |      | |     | |
//     | |  |`\____) |  | || |   _| |_      | || | _/ /    \ \_ | || |  _| |  \ \_  | || |  _| |  \ \_  | |    | | _| |_\/_| |_ | || |     _| |_    | || |  _| |___.' / | || |     _| |_    | |
//     | |  |_______.'  | || |  |_____|     | || ||____|  |____|| || | |____| |___| | || | |____||____| | |    | ||_____||_____|| || |    |_____|   | || | |________.'  | || |    |_____|   | |
//     | |              | || |              | || |              | || |              | || |              | |    | |              | || |              | || |              | || |              | |
//     | '--------------' || '--------------' || '--------------' || '--------------' || '--------------' |    | '--------------' || '--------------' || '--------------' || '--------------' |
//      '----------------'  '----------------'  '----------------'  '----------------'  '----------------'      '----------------'  '----------------'  '----------------'  '----------------' 

//
// Spark MIDI
//
// Open source MIDI control for the Spark 40 amp from Positive Grid
// See    https://github.com/paulhamsh/SparkMIDI for more details
// Also   https://github.com/paulhamsh/Spark for documentation on the API and the message format to the Spark amp   
//

// if CLASSIC is defined, will compile with BLE stack which allows serial bluetooth
// - if not defined will use NimBLE which has no serial bluetooth
// it matters because Android Spark apps use serial bluetooth
// but BLE doesn't handle Spark disconnection well, whereas NimBLE does

// if BLE_MIDI_ON is defined it will allow a BLE MIDI device to connect much like the app

// if BT_CONTROLLER is defined it will search for a bluetooth controller (like Akai LPD8 Wireless or IK Multimedia iRig Blueboard)

#define CLASSIC
#define BLE_MIDI_ON
#define BT_CONTROLLER

#include "SSD1306Wire.h"              // for Thingpulse SSD driver
SSD1306Wire oled(0x3c, 4, 15);        // Default OLED Screen Definitions - ADDRESS, SDA, SCL 

#include "Spark.h"
#include "Screen.h"
#include "MIDI.h"

void setup() {
  Serial.begin(115200);

  oled.init();
  oled.flipScreenVertically();
  oled.clear();
  oled.setFont(ArialMT_Plain_24);
  oled.setTextAlignment(TEXT_ALIGN_CENTER);
  oled.drawString(64, 0, "Spark MIDI");
  oled.setFont(ArialMT_Plain_16);
  oled.setTextAlignment(TEXT_ALIGN_CENTER);
  oled.drawString(64, 26, "0.8");
  oled.setFont(ArialMT_Plain_10);
  oled.drawString(64, 40, "MIDI control for");
  oled.drawString(64, 50, "PG Spark 40"); 
  oled.setTextAlignment(TEXT_ALIGN_LEFT);
  oled.setFont(ArialMT_Plain_10);
  oled.display();

  setup_midi();
     
  delay(3000);  
  setup_screen();
  spark_state_tracker_start();  // set up data to track Spark and app state
}


void loop() {
  byte mi[3];
  int midi_chan, midi_cmd;
  
  show_status();

  if (update_midi(mi)) {
    midi_chan = (mi[0] & 0x0f) + 1;
    midi_cmd = mi[0] & 0xf0;
    
    if (midi_cmd == 0xc0) {       // FCB1010 switches
      switch (mi[1]) {
        case 0:              change_hardware_preset(0);                 break; // pad 1
        case 1:              change_hardware_preset(1);                 break; // pad 2
        case 2:              change_hardware_preset(2);                 break; // pad 3
        case 3:              change_hardware_preset(3);                 break; // pad 4
        case 5:              change_comp_toggle();                      break; // pad 5
        case 6:              change_drive_toggle();                     break; // pad 6
        case 7:              change_delay_toggle();                     break; // pad 7
        case 8:              change_reverb_toggle();                    break; // pad 8
      }
    }
   
    if (midi_cmd == 0x90) {       // LPD8 pads
      switch (mi[1]) {
        case 0x18:           change_hardware_preset(0);                 break; // pad 1
        case 0x1a:           change_hardware_preset(1);                 break; // pad 2
        case 0x1c:           change_hardware_preset(2);                 break; // pad 2
        case 0x1d:           change_hardware_preset(3);                 break; // pad 2        
        case 0x1f:           change_comp_toggle();                      break; // pad 5
        case 0x21:           change_drive_toggle();                     break; // pad 6
        case 0x23:           change_delay_toggle();                     break; // pad 7
        case 0x24:           change_reverb_toggle();                    break; // pad 8
      }
    }

    if (midi_cmd == 0xb0) {       // LPD8 knobs OR FCB 1010 expression
      switch (mi[1]) {
        case 0:              change_amp_param(AMP_GAIN, mi[2]/127.0);   break; // k1       
        case 1:              change_amp_param(AMP_BASS, mi[2]/127.0);   break; // k2       
        case 2:              change_amp_param(AMP_MID, mi[2]/127.0);    break; // k3
        case 3:              change_amp_param(AMP_TREBLE, mi[2]/127.0); break; // k4
        case 4:              change_amp_param(AMP_MASTER, mi[2]/127.0); break; // k5
        case 5:                                                         break; // k6
        case 6:                                                         break; // k7
        case 7:              change_amp_param(AMP_GAIN, mi[2]/127.0);   break; // k8 OR FCB EXP 1
        case 27:             change_amp_param(AMP_MASTER, mi[2]/127.0); break; // FCB EXP2
        case 50:             change_hardware_preset(0);                 break; // pad 1
        case 51:             change_hardware_preset(1);                 break; // pad 2
        case 52:             change_hardware_preset(2);                 break; // pad 3
        case 53:             change_hardware_preset(3);                 break; // pad 4
      }
    }
  
    if (midi_cmd == 0xb3) {       // chan 4 is MIDI Commander
      switch (mi[1]) {
        case 1:              change_amp_param(AMP_GAIN, mi[2]/127.0);   break; // expression pedal l  
        case 2:              change_amp_param(AMP_MASTER, mi[2]/127.0); break; // expression pedal 2  
        case 3:              change_drive_toggle();                     break; // pad 5       
        case 9:              change_mod_toggle();                       break; // pad 6       
        case 14:             change_delay_toggle();                     break; // pad 7
        case 29:             change_reverb_toggle();                    break; // pad 4
        case 22:             change_hardware_preset(0);                 break; // pad 1
        case 23:             change_hardware_preset(1);                 break; // pad 2
        case 24:             change_hardware_preset(2);                 break; // pad 3
        case 105:            change_hardware_preset(3);                 break; // pad 4
        }
    }
  }


  if (update_spark_state()) {
  }

}
