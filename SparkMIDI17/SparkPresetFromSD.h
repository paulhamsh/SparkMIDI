#ifndef SparkPresetFromSD_h
#define SparkPresetFromSD_h
#include "Spark.h"
#include "ArduinoJson.h"
#include <M5Stack.h>
bool loadSparkPresetFromSD(SparkPreset* preset, const char* filename);
bool loadSparkPresetCostum(int genre, int preset);

const char* genreFolders[] = {"Acoustic", "Alternative", "Base", "Blues", "Metal", "Pop", "Rock"};
const int numGenres = 7;

bool loadSparkPresetCustom(int genre, int index);
#endif