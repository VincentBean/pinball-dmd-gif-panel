#include "components/gif/GifLoader/Sequential.hpp"

FsFile root;
FsFile curFile;
String curDirectory;

String Sequential::loadNextFile()
{

    if (!root && !directories.empty())
    {        
        root = sd.open(directories.top());
        curDirectory = curDirectory + "/" + directories.top();
        directories.pop();

        if (!root)
        {
            root.close();
        }
    }

    if (!root)
    {        
        root = sd.open("/gifs");     
        curDirectory = "/gifs";

        if (!root)
        {
            return "";
        }
    }

    curFile.openNext(&root, O_RDONLY);

    if (!curFile)
    {
        curFile.close();
        root.close();

        return Sequential::loadNextFile();
    }

    char name[128];
    curFile.getName(name, 128);

    if (curFile.isDir())
    {
        directories.push(curDirectory + "/" + String(name));
        curFile.close();

        return Sequential::loadNextFile();
    }

    curFile.close();

    return curDirectory + "/" + String(name);
}
