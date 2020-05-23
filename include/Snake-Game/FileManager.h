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

using namespace std;

class FileManager{
private:
    std::ofstream outFile;
    std::ifstream inFile;
public:
    // FileManager();
    // FileManager(string filePath);
    
    void writeMap(const MapItem& map, string filePath);
    MapItem readMap(std::string filePath);

    void writeUser(const UserManager& user, string filePath);
    UserManager readUser(string filePath);
};

#endif