#ifndef FILEREADER_H
#define FILEREADER_H

#include <vector>
#include <string>

class FileReader
{
private:
    std::vector<std::vector<float>> verteces, vertices_normal;
    std::vector<std::vector<int>> polygons, polygons_normal;

    std::vector<float> extractNumbers(const std::string&, int);
    std::pair<std::vector<int>, std::vector<int>> extractPolygons(const std::string&);
public:
    FileReader(std::string);
};

#endif
