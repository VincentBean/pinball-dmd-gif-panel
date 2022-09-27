#include <list>
#include <iterator>
#include "Globals.h"
#include "components/gif/MatrixGif.hpp"
#include "components/gif/GifLoader.hpp"
#include "Helpers.hpp"

bool autoPlay = true;
bool allowNextGif = true;

unsigned long gifStart = 0;
String current_gif;
int gifClockCount = 0;

void nextGif()
{
    if (gifClockCount >= config.showTimeEveryXGif) {
        gifClockCount = 0;
        target_state = SHOW_TIME;
        return;
    }

    current_gif = getNextGif();

    gifStart = millis();

    gifClockCount++;
}

void setGif(String gif)
{
    current_gif = gif;
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

    allowNextGif = autoPlay && millis() - gifStart > config.minGifShowtime;
    if (!gifPlaying && allowNextGif)
    {
        nextGif();
    }

    char gif[current_gif.length() + 1];
    current_gif.toCharArray(gif, sizeof(gif));

    ShowGIF(gif);
}
