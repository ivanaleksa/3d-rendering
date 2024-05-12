#ifndef FILEREADER_H
#define FILEREADER_H

#include <vector>
#include <string>
#include <geometry.h>

class FileReader
{
private:
    std::vector<Vec3d<float>> verteces, vertices_normal;
    std::vector<std::vector<int>> polygons, polygons_normal;

    Vec3d<float> extractNumbers(const std::string&, int);
    std::pair<std::vector<int>, std::vector<int>> extractPolygons(const std::string&);
public:
    FileReader(std::string);

    std::vector<std::vector<int>> getPolygons() { return polygons; }
    std::vector<Vec3d<float>> getVertices() { return verteces; }
    std::vector<Vec3d<float>> getNorms() { return vertices_normal; }
    std::vector<std::vector<int>> getPolygonsNorms() { return polygons_normal; }
};

#endif
