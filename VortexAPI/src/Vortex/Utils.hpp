#include <fstream>
#include "VortexPCH.hpp"

namespace VX
{
    namespace Utils
    {
        static std::vector<char> readFile(const std::string& filename) {
            std::ifstream file(filename, std::ios::ate | std::ios::binary);

            if (!file.is_open()) {
                VX_CORE_ERROR("Utils: Failed to open file!");
                throw std::runtime_error("failed to open file!");
            }
            
            size_t fileSize = (size_t) file.tellg();
            std::vector<char> buffer(fileSize);
            
            file.seekg(0);
            file.read(buffer.data(), fileSize);
            
            file.close();

            return buffer;
        }
    }
}
