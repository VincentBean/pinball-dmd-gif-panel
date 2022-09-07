#ifndef _GIFLOADER_
#define _GIFLOADER_


typedef enum {
    // Files are loaded one by one from the root.  
    SEQUENTIAL = 0,  

    /**
     * An indexing file is created to randomly load the GIFS
     * Best for large amounts of GIFS and randomly loading them
     */  
    INDEXED

} load_strategy_t;

bool queueEmpty();
void countTotalFiles();
void handleGifQueue();
String getNextGif();

#endif