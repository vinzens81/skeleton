#include "SPIFFS.h"
bool writeFile(String name, String content);
bool deleteFile(String name);
String readFile(String name);

String readFile(String name)
{
  String fileName = "/" + name + ".txt";
  File file = SPIFFS.open(fileName);
  if (!file)
  {
    Serial.println("There was an error opening the file for reading");
    return "error reading file";
  }
  String var = "";
  while (file.available())
  {
    var += char(file.read());
  }
  file.close();
  return var;
}

bool deleteFile(String name)
{
  Serial.print("Delete File: ");
  Serial.println(name);
  String fileName = "/" + name + ".txt";
  return SPIFFS.remove(fileName);
}

bool writeFile(String name, String content)
{
  String fileName = "/" + name + ".txt";
  File file = SPIFFS.open(fileName, FILE_WRITE);
  Serial.print("Writing to File: ");
  Serial.print(fileName);
  Serial.print(" with content: ");
  Serial.println(content);
  if (!file)
  {
    Serial.println("There was an error opening the file for writing");
    return false;
  }

  if (file.print(content))
  {
    Serial.println("File was written");
    ;
    file.close();
    return true;
  }
  else
  {
    Serial.println("File write failed");
    file.close();
    return false;
  }
}

String getListOfAllFiles()
{
  String fileListe;
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  while (file)
  {
    fileListe += file.name();
    fileListe += ";";
    file = root.openNextFile();
  }
  return fileListe;
}