#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <Arduino.h>
#include "FS.h"

class FileManager
{
public:
    FileManager();
    static String read_file(String path);
    ~FileManager();
};

#endif
