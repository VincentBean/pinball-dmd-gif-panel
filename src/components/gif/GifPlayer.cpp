#include <list>
#include <iterator>
#include "Globals.h"
#include "components/gif/MatrixGif.hpp"
#include "components/gif/GifLoader.hpp"
#include "Helpers.hpp"

#define DEFAULT_GIF "boot.gif"

bool autoPlay = true;
bool allowNextGif = true;

unsigned long gifStart = 0;
int minPlaytime = 2000; // TODO: Make configurable
String current_gif;
int currentGifIndex = 0;
int missingCount = 0;

void nextGif()
{
    current_gif = getNextGif();

    while (!sd.exists(current_gif)) {
        message("Missing files", true);
        current_gif = getNextGif();

        if (sd.exists(current_gif)) {
            missingCount = 0;
            break;
        }

        missingCount++;

        if (missingCount > 10 && config.loadStrategy == INDEXED) {
            loadStrategy = SEQUENTIAL;            
        }

        delay(100);
    }

    gifStart = millis();
    interruptGif = true;
}

void setGif(String gif)
{
    current_gif = gif;
    interruptGif = true;
}

void handleGif()
{
    if (queueEmpty() && !gifPlaying)
    {
        message("No GIFs", true);
        return;
    }

    if (current_gif.length() == 0)
    {
        nextGif();
    }

    while (!current_gif.endsWith(".gif"))
    {
        nextGif();
    }

    allowNextGif = autoPlay && millis() - gifStart > minPlaytime;
    if (!gifPlaying && allowNextGif)
    {
        nextGif();
    }

    char gif[current_gif.length() + 1];
    current_gif.toCharArray(gif, sizeof(gif));

    ShowGIF(gif);
}
