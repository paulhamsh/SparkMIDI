#include "SparkPresetFromSD.h"


bool loadSparkPresetFromSD(SparkPreset* preset, const char* filename) {
  File configFile = SD.open(filename);
  if (!configFile) {
    DEBUG("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 4096) {
    DEBUG("Config file size is too large");
    return false;
  }

  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  configFile.close();

  DynamicJsonDocument doc(4096);
  DeserializationError error = deserializeJson(doc, buf.get(), size);
  if (error) {
    DEBUG("Failed to parse config file");
    return false;
  }

  // Parse meta data
  DEBUG("Parse meta data...");
  strncpy(preset->UUID, doc["meta"]["id"].as<const char*>(), sizeof(preset->UUID));
  strncpy(preset->Name, doc["meta"]["name"].as<const char*>(), sizeof(preset->Name));
  strncpy(preset->Version, doc["meta"]["version"].as<const char*>(), sizeof(preset->Version));
  strncpy(preset->Description, doc["meta"]["description"].as<const char*>(), sizeof(preset->Description));
  strncpy(preset->Icon, doc["meta"]["icon"].as<const char*>(), sizeof(preset->Icon));
  preset->BPM = doc["bpm"].as<float>();

  const JsonArray& sigpath = doc["sigpath"];
  size_t sigpathSize = sigpath.size();

  // Parse sigpath array
    DEBUG("Parse sigpath array...");
  int i = 0;
  for (auto fx : sigpath) {
    strncpy(preset->effects[i].EffectName, fx["dspId"].as<const char*>(), sizeof(preset->effects[i].EffectName));
    preset->effects[i].OnOff = fx["active"].as<bool>();
    preset->effects[i].NumParameters = fx["params"].size();
    auto paramsArray = fx["params"].as<JsonArray>();
    for (int l = 0; l < preset->effects[i].NumParameters; l++) {
      preset->effects[i].Parameters[l] = paramsArray[l]["value"].as<float>();
    }
    i++;
  }
  return true;
}


bool loadSparkPresetCustom(int genre, int index) {
  if (genre < 1 || genre > numGenres) {
    // Invalid genre
    DEBUG("Invalid genre!");
    return false;
  }
  
  String folderName = "/" + String(genreFolders[genre - 1]);
  
  if (!SD.exists(folderName)) {
    // Genre folder doesn't exist
    DEBUG("Genre folder :" +folderName+ "doesn't exist!");
    File root= SD.open("/");
    //printDirectory(root,0);
    return false;
  }
  
  File folder = SD.open(folderName);
  
  if (!folder.isDirectory()) {
    // Invalid folder
     DEBUG("Invalid folder!");
    folder.close();
    return false;
  }
  
  File presetFolder = folder.openNextFile();
  int count = 0;
  
  while (presetFolder) {
    if (presetFolder.isDirectory()) {
      if (count == index) {
        // Found the correct folder
        DEBUG("Found the correct folder:");
        DEBUG( (folderName+"/"+presetFolder.name()+"/preset.json").c_str());
        SparkPreset preset;

        if (!loadSparkPresetFromSD(&preset, (folderName+"/"+presetFolder.name()+"/preset.json").c_str())) {
            DEBUG("Failed to load preset!");
            return false;
        }
        change_custom_preset(&preset,3);
        presetFolder.close();
        folder.close();
        //debugOutput(&preset);
        return true;
      }
      count++;
    }
    
    presetFolder = folder.openNextFile();
  }
  
  // Couldn't find folder at specified index
  DEBUG("Couldn't find folder at specified index!");
  folder.close();
  return false;
}

void printDirectory(File dir, int numTabs) {
    while(true) {
    File entry = dir.openNextFile();

    if (!entry) {
      dir.rewindDirectory();
      break;
    }
    
    for (uint8_t i=0; i<numTabs; i++)   Serial.print('\t');
    Serial.print(entry.name());
        
    if (entry.isDirectory()) {
      DEBUG("/");
      printDirectory(entry, numTabs+1);
    } 
  else {
      Serial.print("\t\t");
      DEBUG(entry.size());
    }
    entry.close();
  }
}

void debugOutput(SparkPreset* preset) {
  DEBUG("Current Preset: " + String(preset->curr_preset));
  DEBUG("Total Presets: " + String(preset->preset_num));
  DEBUG("UUID: " + String(preset->UUID));
  DEBUG("Name: " + String(preset->Name));
  DEBUG("Version: " + String(preset->Version));
  DEBUG("Description: " + String(preset->Description));
  DEBUG("Icon: " + String(preset->Icon));
  DEBUG("BPM: " + String(preset->BPM));
  
  for (int i = 0; i < 7; i++) {
    DEBUG("Effect " + String(i+1) + " Name: " + String(preset->effects[i].EffectName));
    DEBUG("Effect " + String(i+1) + " On/Off: " + String(preset->effects[i].OnOff));
    DEBUG("Effect " + String(i+1) + " NumParameters: " + String(preset->effects[i].NumParameters));
    for (int j = 0; j < preset->effects[i].NumParameters; j++) {
      DEBUG("Effect " + String(i+1) + " Parameter " + String(j+1) + ": " + String(preset->effects[i].Parameters[j]));
    }
  }
  
  DEBUG("Checksum: " + String(preset->chksum));
}
