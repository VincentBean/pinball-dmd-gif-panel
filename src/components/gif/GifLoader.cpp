#include <queue>
#include <stack>
#include <algorithm>
#include "Globals.h"
#include "Helpers.hpp"
#include "components/gif/GifLoader.hpp"
#include "components/gif/GifLoader/Loader.hpp"
#include "components/gif/GifLoader/Sequential.hpp"
#include "components/gif/GifLoader/Indexed.hpp"

#define MAX_QUEUED_GIFS 32
#define PREVIOUS_GIFS_TRACK_COUNT 10

Sequential sequentialGifLoader;
Indexed indexedGifLoader;
load_strategy_t loadStrategy = INDEXED;;

std::queue<String> gif_queue;
bool queue_populate_requred = true;

unsigned long total_files = 0;

std::stack<FsFile> directories;

std::vector<String> previous;

void InitLoader()
{
    if (indexedGifLoader.indexFileExists() && loadStrategy != INDEXED) {
        loadStrategy = INDEXED;
    }
}

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

    if (loadStrategy == SEQUENTIAL)
    {
        loadedFile = sequentialGifLoader.loadNextFile();
    }

    if (loadStrategy == INDEXED)
    {
        loadedFile = indexedGifLoader.loadNextFile();
    }

    if (loadedFile == "" || std::find(previous.begin(), previous.end(), loadedFile) != previous.end())
    {
        return;
    }

    gif_queue.push(loadedFile);
}

void handleGifQueue()
{
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

    previous.push_back(gif);

    if (previous.size() > PREVIOUS_GIFS_TRACK_COUNT) {
        previous.erase(previous.begin());
    }

    return gif;
}