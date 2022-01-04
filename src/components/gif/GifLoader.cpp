#include <queue>
#include <stack>
#include "Globals.h"
#include "Helpers.hpp"
#include "components/gif/GifLoader.hpp"
#include "components/gif/GifLoader/Loader.hpp"
#include "components/gif/GifLoader/Sequential.hpp"
#include "components/gif/GifLoader/Indexed.hpp"

#define MAX_QUEUED_GIFS 32

Sequential sequentialGifLoader;
Indexed indexedGifLoader;

std::queue<String> gif_queue;
bool queue_populate_requred = true;

unsigned long total_files = 0;

std::stack<FsFile> directories;

bool queueEmpty()
{
    return gif_queue.size() == 0;
}

void resetGifLoader()
{
    total_files = 0;
    queue_populate_requred = false;

    std::queue<String> empty;
    std::swap(gif_queue, empty);
}

/**
 * Add a single item to the queue
 */
void populateGifQueue()
{
    String loadedFile = sequentialGifLoader.loadNextFile();

    if (config.loadStrategy == SEQUENTIAL)
    {
        loadedFile = sequentialGifLoader.loadNextFile();
    }

    if (config.loadStrategy == INDEXED)
    {
        loadedFile = indexedGifLoader.loadNextFile();
    }

    if (loadedFile == "")
    {
        return;
    }

    gif_queue.push(loadedFile);
}

void handleGifQueue()
{
    if (!sd.exists(GIF_DIR)) {     
        message("GIF dir missing", true);
        return;
    }

    if (gif_queue.size() <= (MAX_QUEUED_GIFS - 5))
    {
        queue_populate_requred = true;
    }

    if (!queue_populate_requred)
        return;

    populateGifQueue();

    if (gif_queue.size() >= MAX_QUEUED_GIFS)
        queue_populate_requred = false;
}

String getNextGif()
{
    String gif = gif_queue.front();
    gif_queue.pop();

    return gif;
}