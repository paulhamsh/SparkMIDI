# SparkMIDI
Midi control capability for the Spark 40 Amp   

Needs an ESP32 device, preferrably with USB host capability.   

v6 (SparkMIDI6) uses a 'vanilla' ESP32 (actually from A-Z Delivery, but it seems pretty standard) with a circuit to allow DIN / serial MIDI, bluetooth MIDI and USB Host MIDI.

Otherwise, you can use the M5 Stack Core with USB base, or a Heltec WIFI with an additional USB host board wired to it.   (See https://github.com/paulhamsh/HeltecUSBHost on how to create that.)   

This allows either the Android or IOS apps to connect to the ESP32, and that can have either a bluetooth midi device and/or a wired USB midi device connected, and everything can control the Spark 40 amp - and the app will update to show the changes.   

The last bit requires a workaround because the app doesn't want to receive changes from the ESP32 - it doesn't expect much from the Spark 40 amp. So this uses the fourth preset to hold the current values and updates the app by fooling it into thinking the preset was saved on the amp (as if the preset button was held for a period).   

This also usese the latest SparkIO class and SparkComms and a new wrapper.   

## Arduino IDE install libraries

If using a separate OLED display the code has been tested using the ```ESP8266 and ESP32 driver for SSD1306 displays``` driver by ThingPulse (4.2.1).   
If using M5Stack or Heltec it has its own display and does not require an additional library.   

The code can use two varieties of BLE library - the standard Arduino ESP32 BLE or NimBLE.    
NimBLE is smaller and handles disconnects better, but it doesn't allow for a Classic Bluetooth stack - which is needed if your app is running on Android.   
The code can choose which library using the ```#define CLASSIC``` directive - if that is defined then the code with use the standard BLE, otherwise it will use NimBLE.   

If you wisht to use NimBLE you must install the ```NimBLE-Arduino``` library. The code has been tested with 1.3.1.   

For the classic BLE, the code is dependent on the version of library being used.   
For M5Stack and vanilla ESP32 ('esp32' in the boards manager) the latest versions work (2.0.1 and 2.0.2 respectively).   
For Heltec ESP ('Heltec ESP') the latest package is v0.0.5 which doesn't have the latest BLE library - it has an older version. To use this version please comment out  ```setMTU()``` in SparkComms.ino   

Any questions please ask as an 'issue'.   

## Case for circuit   

<p align="center">
  <img src="https://github.com/paulhamsh/SparkMIDI/blob/main/pics/box1.jpeg" width="200" title="box1">
  <img src="https://github.com/paulhamsh/SparkMIDI/blob/main/pics/box2.jpeg" width="200" title="box2">
  <img src="https://github.com/paulhamsh/SparkMIDI/blob/main/pics/box3.jpeg" width="200" title="box3">
  <img src="https://github.com/paulhamsh/SparkMIDI/blob/main/pics/box4.jpeg" width="200" title="box4">
  <img src="https://github.com/paulhamsh/SparkMIDI/blob/main/pics/box5.jpeg" width="200" title="box5">
  <img src="https://github.com/paulhamsh/SparkMIDI/blob/main/pics/box6.jpeg" width="200" title="box6">
</p>

## Circuit  

<p align="center">
  <img src="https://github.com/paulhamsh/SparkMIDI/blob/main/pics/circuit1.jpeg" width="300" title="circuit1">
  <img src="https://github.com/paulhamsh/SparkMIDI/blob/main/pics/circuit2.jpeg" width="300" title="circuit2">
  <img src="https://github.com/paulhamsh/SparkMIDI/blob/main/pics/circuit3.jpeg" width="300" title="circuit3">
  <img src="https://github.com/paulhamsh/SparkMIDI/blob/main/pics/circuit4.jpeg" width="300" title="circuit4">
</p>

<p align="center">
  <img src="https://github.com/paulhamsh/SparkMIDI/blob/main/pics/Vero.png" width="700" title="vero">
</p>

NOTE the 5v connection shown under the USB Host board is wired to the Host board and not the veroboard - I didn't know how to show that.   

v5 has a nice UI on M5 Stack Core 2. You can see the v6 OLED UI in the box pictures earlier - not quite so pretty.       

<p align="center">
  <img src="https://github.com/paulhamsh/SparkMIDI/blob/main/pics/M5StackUI.jpg" width="200" title="M5 Core 2">
</p>

The API to control the Spark is new for this version - as in this table (Spark.h).   


```
void change_comp_model(char *new_eff);
void change_drive_model(char *new_eff);
void change_amp_model(char *new_eff);
void change_mod_model(char *new_eff);
void change_delay_model(char *new_eff);

void change_noisegate_onoff(bool onoff);
void change_comp_onoff(bool onoff);
void change_drive_onoff(bool onoff);
void change_amp_onoff(bool onoff);
void change_mod_onoff(bool onoff);
void change_delay_onoff(bool onoff);
void change_reverb_onoff(bool onoff);

void change_noisegate_toggle();
void change_comp_toggle();
void change_drive_toggle();
void change_amp_toggle();
void change_mod_toggle();
void change_delay_toggle();
void change_reverb_toggle();

void change_noisegate_param(int param, float val);
void change_comp_param(int param, float val);
void change_drive_param(int param, float val);
void change_amp_param(int param, float val);
void change_mod_param(int param, float val);
void change_delay_param(int param, float val);
void change_reverb_param(int param, float val);

void change_hardware_preset(int pres_num);
void change_custom_preset(SparkPreset *preset, int pres_num);
void update_ui();
```

And the core program to do this looks like:

```
#define CLASSIC

void setup() {
  spark_state_tracker_start();
}

void loop() {
  if (update_spark_state()) {
    // do your own checks and processing here based on changes to Spark state 
  }
  
  // and put other commands here, such as
  
  // change_hardware_preset(3); 
  // change_reverb_toggle();
  // change_amp_param(AMP_GAIN, 0.99);
  
  // change_amp_model("94MatchDCV2");
  // change_drive_model("Booster");  
  // change_mod_model("GuitarEQ6");
  // change_delay_model("DelayMono");
  // update_ui();
  
}
```

```update_ui()``` will force the app ui to refresh to reflect the actual amp.   

## Bluetooth library   

The ```#define CLASSIC``` specifies which bluetooth library to use.   
```CLASSIC``` uses the BLE bluedroid library which will support a classic bluetooth connection (Android app) and BLE (IOS app) - but it doesn't handle loss of connection to Spark whilst using an IOS app well (it is fine with classic ie Android).   
If ```CLASSIC``` is not defined it will use the NimBLE library which will only work with BLE (so not Android), but handles a Spark connection loss properly for BLE.    


| Library | Works with iOS / Android / Both | Handles disconnects |
|--|--|--|
| NimBLE | iOS only | Yes |
| BLE | Both | Partially |

