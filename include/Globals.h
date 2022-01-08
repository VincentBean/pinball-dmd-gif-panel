#ifndef _GLOBAL_
#define _GLOBAL_

#include <SdFat.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include "components/config/Configuration.hpp"

#define GIF_DIR "/gifs"

#define INDEX_FILENAME_PREFIX "index."
#define INDEX_DIRECTORY "/.index"
#define INDEX_SIZE 250

typedef enum {
    OFF = 0, // LED matrix is off
    
    PLAYING_ART, // Looping trough art
    SHOW_TIME,
    STARTUP,

    INDEXING,
    SD_CARD_ERROR,
    NO_FILES,

} frame_status_t;

extern MatrixPanel_I2S_DMA *dma_display;
extern SdFs sd;

extern Config config;
extern bool saveConfig;

extern frame_status_t target_state;
extern unsigned long lastStateChange;

extern bool interruptGif, gifsLoaded;
extern int brightness;
extern bool autoPlay;
extern bool gifPlaying;
extern bool allowNextGif;
extern bool queue_populate_requred;
extern load_strategy_t loadStrategy;
extern unsigned long total_files;
extern String current_gif;

#endif