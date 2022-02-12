
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
// Also   https://github.com/paulhamsh/Spark     for documentation on the API and the message format to the Spark amp   
//

//Dependencies:
// NimBLE               >=1.2.0 (last tested with 1.3.6)
// SSD1306 by Adafruit  >=2.3.0  

// For ESP32 Dev Kit update the preferences for board managers to point to: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
// And load the latest board library - 2.0.2 when built on 23 Jan 2022
// This has the latest BLE code which has setMTU 
// The Heltec WIFI does not have a library supporting setMTU so that is removed by the preprocessor conditional (in SparkComms)


// Select the BLE type 
// -------------------
// if CLASSIC is defined, will compile with BLE stack which allows serial bluetooth
// - if not defined will use NimBLE which has no serial bluetooth
// it matters because Android Spark apps use serial bluetooth
// but BLE doesn't handle Spark disconnection well, whereas NimBLE does

//#define CLASSIC

// Select the BLE devices you need
// -------------------------------
// if BLE_APP_MIDI is defined it will allow a BLE MIDI device to connect (much like the app)
// if BLE_CONTROLLER is defined it will search for a bluetooth controller (like Akai LPD8 Wireless or IK Multimedia iRig Blueboard)

#define BLE_APP_MIDI
#define BLE_CONTROLLER

// Select whether to use external OLED
// -----------------------------------
//#define OLED_ON

// Select the USB host device type
// -------------------------------
// USB_HOST is the generic MAX 2341 host
// USB_TRINKET is a Trinket M0 device programmed as a USB Host

#define USB_HOST
//#define USB_TRINKET

// Select the ESP32 board type
// ---------------------------
// M5STICK, M5CORE2, ESP_DEVKIT, HELTEC_WIFI
// Don't forget to set your board type in Tools -> Board as well!!
//
// Allowed combinations are:
// M5STICK && USB_TRINKET
// M5CORE2 && USB_TRINKET
// M5CORE && USB_HOST  (the USB Host base from M5)
// ESP_DEVKIT && (USB_TRINKET || USB_HOST)  can be either - the only one with a circuit for both so far
// HELTEC_WIFI && USB_HOST
//
// OLED_ON is acceptable with all boards except HELTEC_WIFI

//#define ESP_DEVKIT
#define HELTEC_WIFI
//#define M5CORE
//#define M5CORE2
//#define M5STICK

// Some checks in the pre-processing to ensure no invalid combinations (probably not exhaustive)
// The combinations are only excluded because there has been no sample circuit made with them yet, 
// so GPIO pin assignments are not determined

#if defined HELTEC_WIFI && defined OLED_ON
#error ** Cannot use a Heltec WIFI and have OLED_ON set **
#endif

#if defined USB_TRINKET && defined HELTEC_WIFI
#error ** Cannot use a Heltec WIFI with a Tinket M0 USB Host **
#endif

#if defined USB_HOST && (defined M5STICK || defined M5CORE2)
#error ** Cannot use a M5 board with USB Host **
#endif 

// Board specific #includes

#if defined HELTEC_WIFI
  #include "heltec.h"
#elif defined M5STICK
  #include <M5StickC.h>
#elif defined M5CORE2
  #include <M5Core2.h>
#elif defined M5CORE
  #include <M5Stack.h>
#endif

#include "Spark.h"
#include "Screen.h"
#include "MIDI.h"

void setup() {
#if defined HELTEC_WIFI
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);
#elif defined M5STICK || defined M5CORE2 || defined M5CORE
  M5.begin();
#endif
#if defined M5CORE
  M5.Power.begin();
#endif

  Serial.begin(115200);
  splash_screen();
  setup_midi();
  delay(3000);  
  setup_screen();
  spark_state_tracker_start();  // set up data to track Spark and app state
}


void loop() {
  byte mi[3];
  int midi_chan, midi_cmd;

#if defined M5STICK || defined M5CORE2 || defined M5CORE
  M5.update();
#endif
  
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
        case 0x18:           tuner_on_off(true);                        break; // testing tuner
//        case 0x18:           change_hardware_preset(0);                 break; // pad 1
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

    if (midi_cmd == 0xb0 && mi[2] == 127) {       // Spark Control in MIDI mode (7)
      switch (mi[1]) {
        case 64:             change_hardware_preset(0);                 break; // pad 1
        case 66:             change_hardware_preset(1);                 break; // pad 2
        case 65:             change_hardware_preset(2);                 break; // pad 3
        case 67:             change_hardware_preset(3);                 break; // pad 4
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
