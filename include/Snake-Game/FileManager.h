/* 
 * title: FileManager.h
 * author: ty
 */

#ifndef SNAKE_FILE_MANAGER
#define SNAKE_FILE_MANAGER

#include "Primitives/MapItem.h"
#include "Primitives/MapManager.h"
#include "Primitives/UserItem.h"
#include <fstream>
#include <string>
#include <ctime>

class FileManager{
private:
    std::ofstream outFile;
    std::ifstream inFile;

    bool isFileExist(std::string filePath);
public:    
    void writeMap(const MapItem& map, std::string filePath);
    MapItem readMap(std::string filePath);

    void writeUser(const UserItem &user);
    UserItem readUser(std::string filePath);

    void scanDir(std::string filePath);
};

#endif