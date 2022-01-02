#ifndef _GLOBAL_
#define _GLOBAL_

#include <SdFat.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

#define GIF_DIR "/gifs"

#define INDEX_FILENAME_PREFIX "index."
#define INDEX_DIRECTORY "/.index"
#define INDEX_SIZE 250

typedef enum {
    OFF = 0, // LED matrix is off
    PLAYING_ART, // Looping trough art
    SHOW_TEXT,
    SHOW_TIME,
    STARTUP,

    INDEXING,
    CONNECT_WIFI, // Connecting to WiFi
    ADJ_BRIGHTNESS, // Adjusting brightness
    SD_CARD_ERROR,
    NO_FILES,
    
    // Add new states above this, higher states than this cannot be set by the API!

    OTA_UPDATE,
    ERROR

} frame_status_t;

extern MatrixPanel_I2S_DMA *dma_display;
extern SdFs sd;

extern frame_status_t target_state;
extern unsigned long lastStateChange;

extern int brightness;
extern bool autoPlay;
extern bool gifPlaying;
extern bool allowNextGif;
extern bool queue_populate_requred;
extern unsigned long total_files;
extern String current_gif;

#endif