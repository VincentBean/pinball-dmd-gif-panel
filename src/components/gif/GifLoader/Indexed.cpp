#include "Helpers.hpp"
#include "Globals.h"
#include "components/gif/GifLoader/Indexed.hpp"

String Indexed::generateIndexFilename(int index = 0)
{
    return String(INDEX_DIRECTORY) + "/" + String(INDEX_FILENAME_PREFIX) + String(index);
}

bool Indexed::indexFileExists()
{
    return !sd.exists(generateIndexFilename());
}

std::vector<String> Indexed::readIndexFile(String indexFileName)
{
    FsFile indexFile = sd.open(indexFileName, O_RDONLY);

    std::vector<String> files;

    char line[256];

    indexFile.rewind();

    while (indexFile.available())
    {
        int n = indexFile.fgets(line, sizeof(line));

        // no content or line too long
        if (n <= 0 || (line[n - 1] != '\n' && n == (sizeof(line) - 1)))
        {
            break;
        }

        files.push_back(String(line));
    }

    indexFile.close();

    return files;
}

// Read random item from the index
String Indexed::loadNextFile()
{
    if (!indexesLoaded)
    {
        loadIndexes();
    }

    if (indexes.empty())
    {
        loadStrategy = SEQUENTIAL;
        message("Indexes empty or no index file, switching to sequential");
        return "";
    }

    int index = random(0, indexes.size());

    std::vector<String> files = readIndexFile(indexes.at(index));

    if (files.size() == 0)
        return "";

    int randomFile = random(0, (files.size() - 1));
    String f = files.at(randomFile);
    f.trim();
    f.replace("\n", "");

    return f;
}

void Indexed::loadIndexes()
{
    if (!sd.exists(INDEX_DIRECTORY))
    {
        message("No index dir", true);
        loadStrategy = SEQUENTIAL;
        return;
    }

    FsFile indexRoot = sd.open(INDEX_DIRECTORY);

    if (!indexRoot)
    {
        message("Index root fail", true);
        loadStrategy = SEQUENTIAL;
        return;
    }

    for (;;)
    {
        FsFile indexFile = indexRoot.openNextFile();

        if (!indexFile)
        {
            break;
        }

        char name[256];
        indexFile.getName(name, 256);

        indexes.push_back(String(INDEX_DIRECTORY) + "/" + String(name));

        message("Loaded index file " + String(INDEX_DIRECTORY) + "/" + String(name));

        indexFile.close();
    }

    indexesLoaded = true;
}