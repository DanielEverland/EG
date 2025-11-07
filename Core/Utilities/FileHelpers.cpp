#include "FileHelpers.h"

#include <cassert>
#include <fstream>
#include <iostream>

std::string FileHelpers::LoadFileToString(const std::string& filePath)
{
    std::ifstream file(filePath);
    assert(file.is_open());
    
    std::string toReturn;
    std::string buffer;
    while (std::getline(file, buffer)) {
        toReturn.append(buffer);
    }

    file.close();
    return toReturn;
}
