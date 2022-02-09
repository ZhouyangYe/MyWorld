#include "Util.h"

namespace MyWorld
{
    std::string readText(const std::string& filepath)
    {
        std::string result;
        std::ifstream stream(filepath);
        std::string line;

        while (getline(stream, line))
        {
            result += line + '\n';
        }

        stream.close();
        return result;
    }
}
