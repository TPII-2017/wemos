#include "FileManager.h"

FileManager::FileManager()
{
    if (SPIFFS.begin())
        Serial.println("Todo piola");
    else
        Serial.println("Nada piola");
}

String FileManager::read_file(String path)
{
    String data;

    if (SPIFFS.exists(path))
    {
        Serial.println("The file exists!");
        File f = SPIFFS.open(path, "r");
        if (!f)
            Serial.println("Some thing went wrong trying to open the file...");
        else
        {
            data = f.readString();
            f.close();
        }
    }
    else
        Serial.println("No such file found.");

    return data;
}

FileManager::~FileManager()
{
    SPIFFS.end();
}
