#include <Update.h>
#include "Globals.h"
#include "Helpers.hpp"

#define UPDATE_FILE "/update.bin"

void update(Stream &updateSource, size_t updateSize)
{
    if (!Update.begin(updateSize))
    {
        message("Not enough space", true);
        delay(2000);
        return;
    }

    message("Updating", true);

    size_t written = Update.writeStream(updateSource);

    message(String(written) + "/" + String(updateSize), true);
    delay(1000);


    if (Update.end())
    {
        if (Update.isFinished())
        {
            message("Completed!", true);
        }
        else
        {
            message("Failed", true);
        }

    } else {
        message("Error: " + String(Update.getError()));
        delay(10 * 1000);
    }
    

}

void checkUpdate()
{
    if (!sd.exists(UPDATE_FILE))
    {
        return;
    }

    FsFile updateBin = sd.open(UPDATE_FILE);

    if (updateBin.isDirectory())
    {
        updateBin.close();
        return;
    }

    message("Found update file", true);
    delay(1000);

    size_t size = updateBin.size();

    if (size > 0)
    {
        update(updateBin, size);
    }
    else
    {
        message("update file empty", true);
        delay(1000);
    }

    updateBin.close();

    sd.remove(UPDATE_FILE);
}