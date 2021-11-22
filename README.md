# SparkMIDI
Midi control capability for the Spark 40 Amp   

Needs an ESP32 device, preferrably with USB host capability.   

The M5 Stack Core with USB base is perfect, or a Heltec WIFI with an additional USB host board wired to it.   (See https://github.com/paulhamsh/HeltecUSBHost on how to create that.)   

This allows either the Android or IOS apps to connect to the ESP32, and that can have either a bluetooth midi device and/or a wired USB midi device connected, and everything can control the Spark 40 amp - and the app will update to show the changes.   

The last bit requires a workaround because the app doesn't want to receive changes from the ESP32 - it doesn't expect much from the Spark 40 amp. So this uses the fourth preset to hold the current values and updates the app by fooling it into thinking the preset was saved on the amp (as if the preset button was held for a period).   

This also usese the latest SparkIO class and SparkComms and a new wrapper.   

