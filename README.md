# SparkMIDI
Midi control capability for the Spark 40 Amp   

Needs an ESP32 device, preferrably with USB host capability.   

The M5 Stack Core with USB base is perfect, or a Heltec WIFI with an additional USB host board wired to it.   (See https://github.com/paulhamsh/HeltecUSBHost on how to create that.)   

This allows either the Android or IOS apps to connect to the ESP32, and that can have either a bluetooth midi device and/or a wired USB midi device connected, and everything can control the Spark 40 amp - and the app will update to show the changes.   

The last bit requires a workaround because the app doesn't want to receive changes from the ESP32 - it doesn't expect much from the Spark 40 amp. So this uses the fourth preset to hold the current values and updates the app by fooling it into thinking the preset was saved on the amp (as if the preset button was held for a period).   

This also usese the latest SparkIO class and SparkComms and a new wrapper.   

The API to control the Spark is new for this version - as in this table (Spark.h).   

![Example](https://github.com/paulhamsh/SparkMIDI/blob/main/image1.jpeg)


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

The ```#define CLASSIC``` specifies which bluetooth library to use.   
```CLASSIC``` uses the BLE bluedroid library which will support a classic bluetooth connection (Android app) and BLE (IOS app) - but it doesn't handle loss of connection to Spark whilst using an IOS app well (it is fine with classic ie Android).   
If ```CLASSIC``` is not defined it will use the NimBLE library which will only work with BLE, but handles a Spark connection loss properly for BLE.    
