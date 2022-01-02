#include "components/gif/GifLoader/Indexed.hpp"

String Indexed::generateIndexFilename(int index = 0)
{
    return String(INDEX_DIRECTORY) + "/" + String(INDEX_FILENAME_PREFIX) + String(index);
}

bool Indexed::indexingRequired()
{
    String name = generateIndexFilename();

    bool required = !sd.exists(name);

    Serial.println("Checking if indexing is required");

    Serial.print(name);

    if (required) {
        Serial.println(" YES");
    } else {
        Serial.println(" NO");
    }

    return required;
}

// https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

std::vector<String> Indexed::readIndexFile(String indexFileName)
{
    FsFile indexFile = sd.open(indexFileName, O_RDONLY);

    indexFile.printFileSize(&Serial);
    indexFile.printName(&Serial);

    std::vector<String> files;

    char line[256];

    indexFile.rewind();

    while (indexFile.available())
    {
        int n = indexFile.fgets(line, sizeof(line));

        // no content or line too long
        if (n <= 0 || (line[n-1] != '\n' && n == (sizeof(line) - 1))) {
            break;
        }

        files.push_back( String(line));
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

    if (indexing || indexingRequired())
    {
        index();
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

    return String(GIF_DIR) + String("/") + f;
}

void Indexed::writeIndex()
{
    String fullPath = generateIndexFilename(currentIndex);

    if (sd.exists(fullPath))
    {
        sd.remove(fullPath);
    }

    FsFile indexFile = sd.open(fullPath, O_RDWR | O_CREAT | O_AT_END);

    for (int i = 0; i < indexFiles.size(); i++)
    {
        indexFile.println(indexFiles.at(i));
    }

    indexFile.flush();
    indexFile.close();
    indexFiles.clear();

    indexes.push_back(fullPath);

    currentIndex++;
}

void Indexed::loadIndexes()
{
    if (!sd.exists(INDEX_DIRECTORY))
    {
        return;
    }

    FsFile indexRoot = sd.open(INDEX_DIRECTORY);

    if (!indexRoot)
    {
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

        indexFile.close();
    }

    indexesLoaded = true;
}

void Indexed::index()
{
    target_state = INDEXING;

    if (!curDirectory)
    {
        curDirectory = sd.open(GIF_DIR);

        if (sd.exists(INDEX_DIRECTORY))
        {
            sd.rmdir(INDEX_DIRECTORY);
        }

        sd.mkdir(INDEX_DIRECTORY);

        indexing = true;
        total_files = 0;
        indexFiles.clear();

        while (!directories.empty())
            directories.pop();
    }

    FsFile nextFile = curDirectory.openNextFile();

    if (!nextFile)
    {
        nextFile.close();
        curDirectory.close();

        if (!directories.empty())
        {
            String nextDir = directories.top();
            curDirectory = sd.open(nextDir);
            directories.pop();

            return;
        }

        if (indexFiles.size() > 0)
            writeIndex();

        target_state = PLAYING_ART;
        indexing = false;

        return;
    }

    char name[256];
    nextFile.getName(name, 256);

    if (nextFile.isDirectory())
    {
        directories.push(String(name));

        nextFile.close();
        return;
    }

    String nextFileName = String(name);

    if (!nextFileName.endsWith(".gif"))
    {
        nextFile.close();
        return;
    }

    indexFiles.push_back(nextFileName);
    total_files++;
    nextFile.close();

    if (indexFiles.size() >= INDEX_SIZE)
    {
        writeIndex();
    }
}