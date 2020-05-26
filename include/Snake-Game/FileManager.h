/* 
 * title: FileManager.h
 * author: 
 */

#ifndef SNAKE_FILE_MANAGER
#define SNAKE_FILE_MANAGER

#include "Primitives/MapItem.h"
#include "Primitives/MapManager.h"
#include "Primitives/UserManager.h"
#include <fstream>
#include <string>

class FileManager{
private:
    std::ofstream outFile;
    std::ifstream inFile;
public:
    // FileManager();
    // FileManager(string filePath);
    
    void writeMap(const MapItem& map, std::string filePath);
    MapItem readMap(std::string filePath);

    void writeUser(const UserManager& user, std::string filePath);
    UserManager readUser(std::string filePath);
};

#endif