#include "filereader.h"

#include <fstream>
#include <sstream>
#include <QDebug>
#include <qDir>

FileReader::FileReader(std::string fileName)
{
    std::string line;
    std::ifstream in(fileName);

    if (in.is_open())
    {
        while (std::getline(in, line))
        {
            if (line[0] == 'v' && line[1] == ' ')
                verteces.push_back(extractNumbers(line, 2));

            if (line[0] == 'v' && line[1] == 'n')
                vertices_normal.push_back(extractNumbers(line, 3));

            if (line[0] == 'f' && line[1] == ' ')
            {
                std::pair<std::vector<int>, std::vector<int>> vertex_coords = extractPolygons(line);
                polygons.push_back(vertex_coords.first);
                polygons_normal.push_back(vertex_coords.second);
            }
        }
    }

    in.close();
}

Vec3d<float> FileReader::extractNumbers(const std::string& line, int countMissingChars)
{
    std::vector<float> numbers;
    std::istringstream iss(line);

    // miss first chars
    char ch;
    for (int i = 0; i < countMissingChars - 1; ++i)
        iss >> ch;

    // read needed numbers
    float num;
    while (iss >> num) {
        numbers.push_back(num);
    }

    Vec3d<float> res = Vec3d(numbers[0], numbers[1], numbers[2]);
    return res;
}

std::pair<std::vector<int>, std::vector<int>> FileReader::extractPolygons(const std::string& line)
{
    std::istringstream iss(line);

    char ch;
    iss >> ch;

    int num;
    char slash;
    std::vector<int> vertices_group, normal_group;
    while (iss >> num)
    {
        vertices_group.push_back(num - 1);

        iss >> slash >> num;

        iss >> slash >> num;
        normal_group.push_back(num - 1);
    }

    return std::make_pair(vertices_group, normal_group);
}

std::vector<std::vector<int>> FileReader::getPolygons()
{
    return polygons;
}

std::vector<Vec3d<float>> FileReader::getVertices()
{
    return verteces;
}
