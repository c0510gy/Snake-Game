/* 
 * title: FileManager.h
 * author: 
 */

#ifndef SNAKE_FILE_MANAGER
#define SNAKE_FILE_MANAGER

#include "MapManager.h"
#include "UserManager.h"
#include <fstream>
#include <string>

class FileManager{
private:
    std::ofstream outFile;
    std::ifstream inFile;
public:
    FileManager();
    
    void writeMap(const MapManager& map, std::string filePath);
    MapManager readMap(std::string filePath);

    void writeUser(const UserManager& user, std::string filePath);
    UserManager readUser(std::string filePath);
};

#endif