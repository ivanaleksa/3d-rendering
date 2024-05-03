#ifndef FILEREADER_H
#define FILEREADER_H

#include <vector>
#include <string>

class FileReader
{
private:
    std::vector<float> verteces;
    std::vector<std::vector<int>> polygons;
public:
    FileReader(std::string);
};

#endif
