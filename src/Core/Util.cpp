#include "Util.h"

namespace MyWorld
{
    namespace Util {
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

        void log(char* stir)
        {
            std::cout << stir << "\n";
        }

        void log(glm::vec3 vec, char* text)
        {
            std::cout << text << vec.x << ", " << vec.y << ", " << vec.z << "\n";
        }

        void log(float num, char* text)
        {
            std::cout << text << num << "\n";
        }

        void log(double num, char* text)
        {
            std::cout << text << num << "\n";
        }

        void log(bool boo, char* text)
        {
            std::cout << text << boo << "\n";
        }

        void log(uint8_t type, char* text)
        {
            std::cout << text << std::bitset<8>(type) << "\n";
        }
    }
}
