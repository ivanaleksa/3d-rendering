#include "filereader.h"

#include <fstream>

FileReader::FileReader(std::string fileName)
{
    std::string line;
    std::ifstream in(fileName);

    if (in.is_open())
    {
        while (std::getline(in, line))
        {

        }
    }

    in.close();
}
